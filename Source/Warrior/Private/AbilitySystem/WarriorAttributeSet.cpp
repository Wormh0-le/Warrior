// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/HeroUIComponent.h"
#include "Components/UI/PawnUIComponent.h"

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
	PawnUIInterface = PawnUIInterface.IsValid() ? PawnUIInterface : TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	checkf(PawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface!"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	UPawnUIComponent* PawnUIComponent = PawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("Couldn't extract PawnUIComponent from %s!"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute()) {
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		PawnUIComponent->OnHeathChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute()) {
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);
		if (GetCurrentRage() == GetMaxRage())
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
		} else if (GetCurrentRage() == 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Empty);
		} else
		{
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Empty);
		} 
		UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent();
		checkf(HeroUIComponent, TEXT("Couldn't extract HeroUIComponent from %s!"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
		HeroUIComponent->OnRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
	}
	if (Data.EvaluatedData.Attribute == GetOutputDamageAttribute())
	{
		// const FString BeforeDamageDebugMsg = FString::Printf(
		// 	TEXT("PostGameplayEffectExecute: CurrentHealth: %.2f"),
		// 	GetCurrentHealth()
		// );
		// Debug::Print(BeforeDamageDebugMsg);
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth() - GetOutputDamage(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		PawnUIComponent->OnHeathChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
		// const FString AfterDamageDebugMsg = FString::Printf(
		// 	TEXT("PostGameplayEffectExecute: CurrentHealth: %.2f, MaxHealth: %.2f, OutputDamage: %.2f"),
		// 	GetCurrentHealth(), GetMaxHealth(), GetOutputDamage()
		// );
		// Debug::Print(AfterDamageDebugMsg);
		
		if (GetCurrentHealth() <= 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);
		}
	}
}
