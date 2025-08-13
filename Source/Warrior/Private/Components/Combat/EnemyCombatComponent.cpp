// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"

#include "WarriorGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"


void UEnemyCombatComponent::OnWeaponHitTargetActor(AActor* TargetActor)
{
	if (OverlappedActors.Contains(TargetActor))	return;
	OverlappedActors.AddUnique(TargetActor);

	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHasTag(TargetActor, WarriorGameplayTags::Player_Status_Blocking);
	const bool bIsAttackUnblockable = UWarriorFunctionLibrary::NativeDoesActorHasTag(GetOwningPawn(), WarriorGameplayTags::Enemy_Status_Unblockable);

	if (bIsPlayerBlocking && !bIsAttackUnblockable)
	{
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), TargetActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = TargetActor;
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			TargetActor,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			EventData
		);
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
