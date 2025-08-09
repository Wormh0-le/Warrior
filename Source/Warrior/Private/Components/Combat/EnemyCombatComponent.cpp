// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"

#include "WarriorGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"


void UEnemyCombatComponent::OnWeaponHitTargetActor(AActor* TargetActor)
{
	if (OverlappedActors.Contains(TargetActor))	return;
	OverlappedActors.AddUnique(TargetActor);
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsAttackUnblockable)
	{
		//TODO: check if the block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = TargetActor;
	if (bIsValidBlock)
	{
		//TODO: Handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}

void UEnemyCombatComponent::OnWeaponPullback(AActor* TargetActor)
{
	Super::OnWeaponPullback(TargetActor);
}
