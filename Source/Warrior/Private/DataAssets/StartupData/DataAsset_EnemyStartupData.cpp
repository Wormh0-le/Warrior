// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "DataAssets/StartupData/DataAsset_EnemyStartupData.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"


void UDataAsset_EnemyStartupData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	for (const TSubclassOf<UWarriorEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
	{
		if (!AbilityClass)	continue;
		FGameplayAbilitySpec AbilitySpec{AbilityClass};
		AbilitySpec.SourceObject  = InASCToGive->GetAvatarActor();
		AbilitySpec.Level         = ApplyLevel;
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
