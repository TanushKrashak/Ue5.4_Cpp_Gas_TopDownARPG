// No Copyright!

#include "AbilitySystem/Cpp_AbilityTypes_RPG.h"


UScriptStruct* FGameplayEffectContext_RPG::GetScriptStruct() const {
	return StaticStruct();
}

FGameplayEffectContext* FGameplayEffectContext_RPG::Duplicate() const {
	FGameplayEffectContext_RPG* NewContext = new FGameplayEffectContext_RPG(*this);
	*NewContext = *this;
	if (GetHitResult()) {
		// Does a deep copy of the HitResult
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}

bool FGameplayEffectContext_RPG::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) {	
	uint32 RepBits = 0;
	// |= 1 << 0 basically means that the first bit is set to 1, first as in the least significant bit
	// &  1 << 0 basically checks if the first bit is set to 1, first as in the least significant bit

	if (Ar.IsSaving()) {
		if (bReplicateInstigator && Instigator.IsValid()) {
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid()) {
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid()) {
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid()) {
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0) {
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid()) {
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin) {
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit) {
			RepBits |= 1 << 7;
		}
		if (bIsCritialHit) {
			RepBits |= 1 << 8;
		}
		// Debuff
		if (bIsSuccessfulDebuff) {
			RepBits |= 1 << 9;
		}
		if (DebuffDamage >= 0.0f) {
			RepBits |= 1 << 10;
		}
		if (DebuffFrequency >= 0.0f) {
			RepBits |= 1 << 11;
		}
		if (DebuffDuration >= 0.0f) {
			RepBits |= 1 << 12;
		}
        if (DamageType.IsValid()) {
			RepBits |= 1 << 13;
		}
		
		if (!DeathImpulse.IsZero()) {
			RepBits |= 1 << 14;
		}
		if (!KnockbackImpulse.IsZero()) {
			RepBits |= 1 << 15;
		}
	}
	Ar.SerializeBits(&RepBits, 15);

	/* 
	*  MEANING OF Ar << Variable:
	* 
	* Serialization: When Ar is in "write" mode, this stores the value of bIsCritialHit in the archive so that it 
	*					can be sent over the network.
	* Deserialization: When Ar is in "read" mode, this retrieves the value from the archive and sets bIsCritialHit
	*					accordingly.
	*/
	if (RepBits & (1 << 0)) {
		Ar << Instigator;
	}
	if (RepBits & (1 << 1)) {
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2)) {
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3)) {
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4)) {
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5)) {
		if (Ar.IsLoading()) {
			if (!HitResult.IsValid()) {
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6)) {
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else {
		bHasWorldOrigin = false;
	}

	if (Ar.IsLoading()) {
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	

	if (RepBits & (1 << 7)) {
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8)) {
		Ar << bIsCritialHit;
	}
	// Debuff
	if (RepBits & (1 << 9)) {
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 10)) {
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11)) {
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 12)) {
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 13)) {
		if (Ar.IsLoading()) {
			if (!DamageType.IsValid()) {
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14)) {
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15)) {
		KnockbackImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	bOutSuccess = true;
	return true;
}

bool FGameplayEffectContext_RPG::IsBlockedHit() const {
	return bIsBlockedHit;
}
void FGameplayEffectContext_RPG::SetBlockedHit(const bool bNewBlock) {
	bIsBlockedHit = bNewBlock;
}

bool FGameplayEffectContext_RPG::IsCriticalHit() const {
	return bIsCritialHit;
}
void FGameplayEffectContext_RPG::SetCriticalHit(const bool bNewCrit) {
	bIsCritialHit = bNewCrit;
}

bool FGameplayEffectContext_RPG::IsSuccessfulDebuff() const {
	return bIsSuccessfulDebuff;
}
float FGameplayEffectContext_RPG::GetDebuffDamage() const {
	return DebuffDamage;
}
float FGameplayEffectContext_RPG::GetDebuffFrequency() const {
	return DebuffFrequency;
}
float FGameplayEffectContext_RPG::GetDebuffDuration() const {
	return DebuffDuration;
}
TSharedPtr<FGameplayTag> FGameplayEffectContext_RPG::GetDamageType() const {
	return DamageType;
}

void FGameplayEffectContext_RPG::SetIsSuccessfulDebuff(const bool bNewDebuff) {
	bIsSuccessfulDebuff = bNewDebuff;
}
void FGameplayEffectContext_RPG::SetDebuffDamage(const float NewDebuffDamage) {
	DebuffDamage = NewDebuffDamage;
}
void FGameplayEffectContext_RPG::SetDebuffFrequency(const float NewDebuffFrequency) {
	DebuffFrequency = NewDebuffFrequency;
}
void FGameplayEffectContext_RPG::SetDebuffDuration(const float NewDebuffDuration) {
	DebuffDuration = NewDebuffDuration;
}
void FGameplayEffectContext_RPG::SetDamageType(const TSharedPtr<FGameplayTag> InDamageType) {
	DamageType = InDamageType;
}

FVector FGameplayEffectContext_RPG::GetDeathImpulse() const {
	return DeathImpulse;
}
void FGameplayEffectContext_RPG::SetDeathImpulse(const FVector& NewDeathImpulse) {
	DeathImpulse = NewDeathImpulse;
}

FVector FGameplayEffectContext_RPG::GetKnockbackImpulse() const {
	return KnockbackImpulse;
}
void FGameplayEffectContext_RPG::SetKnockbackImpulse(const FVector& NewKnockbackImpulse) {
	KnockbackImpulse = NewKnockbackImpulse;
}

