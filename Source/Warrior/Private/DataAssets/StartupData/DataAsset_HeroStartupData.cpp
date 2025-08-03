// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "DataAssets/StartupData/DataAsset_HeroStartupData.h"

#include "AbilityStystem/WarriorAbilitySystemComponent.h"
#include "AbilityStystem/Abilities/WarriorGameplayAbility.h"


bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

void UDataAsset_HeroStartupData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	for (const FWarriorHeroAbilitySet& AbilitySet : HeroStartUpAbilitySets)
	{
		if (AbilitySet.IsValid())
		{
			FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
