// No Copyright!


#include "Core/Cpp_PS_Player.h"
#include "AbilitySystem/Cpp_ASC_Master.h"
#include "AbilitySystem/Cpp_ASet_Master.h"
#include "Net/UnrealNetwork.h"

ACpp_PS_Player::ACpp_PS_Player() {
	NetUpdateFrequency = 100.0f;

	// Setup Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UCpp_ASC_Master>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);	
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Setup Attributes Set
	AttributeSet = CreateDefaultSubobject<UCpp_ASet_Master>(TEXT("Attribute Set"));
}

void ACpp_PS_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACpp_PS_Player, Level);
	DOREPLIFETIME(ACpp_PS_Player, Experience);
	DOREPLIFETIME(ACpp_PS_Player, AttributePoints);
	DOREPLIFETIME(ACpp_PS_Player, SpellPoints);
}


UAbilitySystemComponent* ACpp_PS_Player::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

UAttributeSet* ACpp_PS_Player::GetAttributeSet() const {
	return AttributeSet;
}

int32 ACpp_PS_Player::GetPlayerLevel() const {
	return Level;
}	
void ACpp_PS_Player::SetLevel(int32 NewLevel) {
	Level = NewLevel;
	OnLevelChanged.Broadcast(Level);
}
void ACpp_PS_Player::UpdateLevel(int32 LevelChange) {
	SetLevel(Level + LevelChange);
}

int32 ACpp_PS_Player::GetExperience() const {
	return Experience;
}
void ACpp_PS_Player::SetExperience(int32 NewExperience) {
	Experience = NewExperience;
	OnExperienceChanged.Broadcast(Experience);
}
void ACpp_PS_Player::UpdateExperience(int32 ExperienceChange) {
	SetExperience(Experience + ExperienceChange);
}

int32 ACpp_PS_Player::GetAttributePoints() const {
	return AttributePoints;
}
void ACpp_PS_Player::SetAttributePoints(int32 NewAttributePoints) {
	AttributePoints = NewAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
void ACpp_PS_Player::UpdateAttributePoints(int32 AttributePointsChange) {
	SetAttributePoints(AttributePoints + AttributePointsChange);
}

int32 ACpp_PS_Player::GetSpellPoints() const {
	return SpellPoints;
}
void ACpp_PS_Player::SetSpellPoints(int32 NewSpellPoints) {
	SpellPoints = NewSpellPoints;
	OnSpellPointsChanged.Broadcast(SpellPoints);
}
void ACpp_PS_Player::UpdateSpellPoints(int32 SpellPointsChange) {
	SetSpellPoints(SpellPoints + SpellPointsChange);
}

void ACpp_PS_Player::OnRep_Level(int32 OldLevel) {
	// For Clients
	OnLevelChanged.Broadcast(Level);
}
void ACpp_PS_Player::OnRep_Experience(int32 OldExperience) {
	// For Clients
	OnExperienceChanged.Broadcast(Experience);
}
void ACpp_PS_Player::OnRep_AttributePoints(int32 OldAttributePoints) {
	// For Clients
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
void ACpp_PS_Player::OnRep_SpellPoints(int32 OldSpellPoints) {
	// For Clients
	OnSpellPointsChanged.Broadcast(SpellPoints);
}

