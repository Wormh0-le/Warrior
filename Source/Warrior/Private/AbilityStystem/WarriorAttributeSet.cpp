// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Warrior/Public/AbilitySystem/WarriorAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "WarriorDebugHelper.h"


UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefencePower(1.f);
	InitOutputDamage(0.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute()) {
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
	}
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute()) {
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);
	}
	if (Data.EvaluatedData.Attribute == GetOutputDamageAttribute())
	{
		const FString BeforeDamageDebugMsg = FString::Printf(
			TEXT("PostGameplayEffectExecute: CurrentHealth: %.2f"),
			GetCurrentHealth()
		);
		Debug::Print(BeforeDamageDebugMsg);
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth() - GetOutputDamage(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		const FString AfterDamageDebugMsg = FString::Printf(
			TEXT("PostGameplayEffectExecute: CurrentHealth: %.2f, MaxHealth: %.2f, OutputDamage: %.2f"),
			GetCurrentHealth(), GetMaxHealth(), GetOutputDamage()
		);
		Debug::Print(AfterDamageDebugMsg);
		//TODO: Notify UI
		//TODO: death
		if (NewCurrentHealth <= 0.f)
		{
		}
	}
}
