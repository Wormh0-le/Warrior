// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "WarriorTypes/WarriorStructTypes.h"

#include "AbilityStystem/Abilities/WarriorHeroGameplayAbility.h"


bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
