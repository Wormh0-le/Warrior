// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilityStystem/WarriorAbilitySystemComponent.h"


void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())	return;
	for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}
