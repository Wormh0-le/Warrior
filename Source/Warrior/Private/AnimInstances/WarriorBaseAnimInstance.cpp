// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AnimInstances/WarriorBaseAnimInstance.h"

#include "GameplayTagContainer.h"
#include "WarriorFunctionLibrary.h"


bool UWarriorBaseAnimInstance::DoesOwnerHasTag(FGameplayTag InTag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UWarriorFunctionLibrary::NativeDoesActorHasTag(OwningPawn, InTag);
	}
	return false;
}
