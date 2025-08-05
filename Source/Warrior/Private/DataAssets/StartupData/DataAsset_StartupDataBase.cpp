// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "DataAssets/StartupData/DataAsset_StartupDataBase.h"

#include "Warrior/Public/AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Warrior/Public/AbilitySystem/Abilities/WarriorGameplayAbility.h"

void UDataAsset_StartupDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	check(InASCToGive);
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	for (const TSubclassOf<UGameplayEffect>& EffectClass : StartupGameplayEffects)
	{
		if (!EffectClass) continue;
		
		UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
		InASCToGive->ApplyGameplayEffectToSelf(
			EffectCDO,
			ApplyLevel,
			InASCToGive->MakeEffectContext()
		);
	}
}

void UDataAsset_StartupDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive,
	UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())	return;
	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
