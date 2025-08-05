// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "WarriorTypes/WarriorStructTypes.h"

#include "Warrior/Public/AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"


bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
