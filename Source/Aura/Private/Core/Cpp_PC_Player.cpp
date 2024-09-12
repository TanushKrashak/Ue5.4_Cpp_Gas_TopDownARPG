// No Copyright!

// Game Includes
#include "Core/Cpp_PC_Player.h"
#include "Interfaces/Cpp_InteractionInterface.h"
#include "Inputs/Cpp_InputComponent_RPG.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "Cpp_AllGameplayTags.h"
#include "UI/WidgetComponents/Cpp_WGTComp_DamageText.h"

// Engine Includes
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"

ACpp_PC_Player::ACpp_PC_Player() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;


	// Multiplayer
	bReplicates = true;

	// Auto Run
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));

}

void ACpp_PC_Player::BeginPlay() {
	Super::BeginPlay();

	// Setup Custom Input Mode of Game and UI
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	// Checks if the Input Mapping Context is valid
	check(InputMappingContext);

	// Get the Enhanced Input Local Player Subsystem and add the Input Mapping Context to it
	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	

}
void ACpp_PC_Player::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CursorTrace();
	
	AutoRun();
}
void ACpp_PC_Player::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Checks if the Input Component is valid, if not, crashes the game
	UCpp_InputComponent_RPG* RPGInputComponent = CastChecked<UCpp_InputComponent_RPG>(InputComponent);


	// Setup the Input Actions
	//========================================================== 
	// Move Action
	RPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACpp_PC_Player::Move);

	// Shift Action
	RPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ACpp_PC_Player::ShiftSPressed);
	RPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ACpp_PC_Player::ShiftReleased);
	//========================================================== 


	// Setup the Ability Input Actions
	RPGInputComponent->BindAbilityActions(InputConfig, this, &ACpp_PC_Player::AbilityInputTagPressed, &ACpp_PC_Player::AbilityInputTagReleased, &ACpp_PC_Player::AbilityInputTagHeld);


	
}

void ACpp_PC_Player::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
													 const bool bBlockedHit, const bool bIsCriticalHit) {
	if (IsValid(TargetCharacter) && DamageTextWidgetCompClass && IsLocalController()) {
		UCpp_WGTComp_DamageText* DamageText = NewObject<UCpp_WGTComp_DamageText>(TargetCharacter,
																				 DamageTextWidgetCompClass);
		if (DamageText) {
			DamageText->RegisterComponent();
			// Just attach and detach so that its location is updated
			DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), 
										  FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			
			DamageText->SetDamageText(DamageAmount, bBlockedHit, bIsCriticalHit);
		}

	}
}

void ACpp_PC_Player::Move(const FInputActionValue& Value) {

	// If the Player is Blocking Input, return
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAllGameplayTags::Get().Player_Block_Input_Pressed)) {
		return;
	}

	// Converts the Input Into a 2d Vector
	const FVector2D InputAxisVector = Value.Get<FVector2D>();	
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);

	// Get Forward Direction & Right Direction
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Move the Pawn
	if (APawn* ControlledPawn = GetPawn()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ACpp_PC_Player::CursorTrace() {	
	// If the Player is Blocking Cursor Trace, return
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAllGameplayTags::Get().Player_Block_CursorTrace)) {
		// if something is already highlighted, unhighlight it
		if (LastActor) {
			LastActor->UnhighlightActor();
		}
		if (CurrentActor) {
			CurrentActor->UnhighlightActor();
		}
		LastActor = CurrentActor = nullptr;
		return;
	}


	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);		

	// If the Cursor does not hit anything, return
	if (!CursorHitResult.bBlockingHit) return;

	// Setup the Last and Current Actor
	LastActor = CurrentActor;
	CurrentActor = CursorHitResult.GetActor();

	/* Line Trace From Cursor
	* There are several scenarios:
	* A. Last & Current Actor are both nullptr
	*	- Do Nothing
	* B. Last Actor is nullptr, Current Actor is valid
	*	- Highlight Current Actor
	* C. Last Actor is valid, Current Actor is nullptr
	*	- Un-highlight Last Actor
	* D. Both are Valid but different
	*	- Un-highlight Last Actor and Highlight Current Actor
	* E. Both are the same and valid
	*	- Keep Highlighting
	*/
	if (LastActor != CurrentActor) {
		if (LastActor) {
			LastActor->UnhighlightActor();
		}
		if (CurrentActor) {
			CurrentActor->HighlightActor();
		}
	}
}

void ACpp_PC_Player::AbilityInputTagPressed(FGameplayTag Tag) {

	// If the Player is Blocking Input, return
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAllGameplayTags::Get().Player_Block_Input_Pressed)) {
		return;
	}

	// Left Mouse Button
	if (Tag.MatchesTagExact(FAllGameplayTags::Get().InputTag_LMB)) {
		bIsTargeting = CurrentActor ? true : false;
		bIsAutoRunning = false;
	}
	if (GetASC()) {
		GetASC()->AbilityInputTagPressed(Tag);
	}
}
void ACpp_PC_Player::AbilityInputTagReleased(FGameplayTag Tag) {

	// If the Player is Blocking Input, return
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAllGameplayTags::Get().Player_Block_Input_Released)) {
		return;
	}

	// Not Left Mouse Button
	if (!Tag.MatchesTagExact(FAllGameplayTags::Get().InputTag_LMB)) {
		if (GetASC()) {
			GetASC()->AbilityInputTagReleased(Tag);
		}
		return;
	}

	GetASC()->AbilityInputTagReleased(Tag);

	// Left Mouse Button
	if (!bIsTargeting && !bIsShiftKeyDown) {
		// If the Follow Time is Less than the Short Press Threshold && Pawn is Valid, Auto Run
		const APawn* ControlledPawn = GetPawn();
		if (ControlledPawn && FollowTime <= ShortPressThreshold) {
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination)) {
				// Draw the Path on the Spline Component
				SplineComponent->ClearSplinePoints();
				for (const auto& PointLoc : NavPath->PathPoints) {
					SplineComponent->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				// Go to the Last Point of the Path, which is a valid destination
				if (NavPath->PathPoints.Num() > 0)
					CachedDestination = NavPath->PathPoints.Last();
				bIsAutoRunning = true;
			}
			// Only Spawn the Niagara System if the Player is not Blocking Input
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAllGameplayTags::Get().Player_Block_Input_Pressed)) {
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OnClickNiagaraSystem, CachedDestination);
			}
		}
		else {
			FollowTime = 0.0f;
			bIsTargeting = false;
		}	
	}
}
void ACpp_PC_Player::AbilityInputTagHeld(FGameplayTag Tag) {

	// If the Player is Blocking Input, return
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAllGameplayTags::Get().Player_Block_Input_Pressed)) {
		return;
	}

	// Not Left Mouse Button
	if (!Tag.MatchesTagExact(FAllGameplayTags::Get().InputTag_LMB)) {
		if (GetASC()) {
			GetASC()->AbilityInputTagHeld(Tag);
		}
		return;
	}
	if (bIsTargeting || bIsShiftKeyDown) {
		GetASC()->AbilityInputTagHeld(Tag);
	}
	// Left Mouse Button
	else {
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHitResult.bBlockingHit) {
			CachedDestination = CursorHitResult.ImpactPoint;
		}

		// If Pawn is Valid, Move the Pawn Towards the Destination
		if (APawn* ControlledPawn = GetPawn()) {
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	return;
}

UCpp_ASC_Master* ACpp_PC_Player::GetASC() {
	if (RPGAbilitySystemComponent == nullptr) {
		// Get the Ability System Component
		RPGAbilitySystemComponent = Cast<UCpp_ASC_Master>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return RPGAbilitySystemComponent;
}

void ACpp_PC_Player::AutoRun() {
	if (!bIsAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn()) {
		// Get Closest Location on Spline
		const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

		// Get Direction
		const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		// Check if should stop Auto Running
		const float DistanceToDestination = FVector::Dist(LocationOnSpline, CachedDestination);
		if (DistanceToDestination <= AutoRunAcceptanceRadius) {
			bIsAutoRunning = false;
		}
	}
}

void ACpp_PC_Player::ShiftSPressed() {
	bIsShiftKeyDown = true;
}

void ACpp_PC_Player::ShiftReleased() {
	bIsShiftKeyDown = false;
}
