// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "WarriorTypes/WarriorStructTypes.h"

#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"


bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
