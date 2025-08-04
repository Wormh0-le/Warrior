// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "WarriorTypes/WarriorStructTypes.h"

#include "AbilityStystem/Abilities/WarriorGameplayAbility.h"


bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
