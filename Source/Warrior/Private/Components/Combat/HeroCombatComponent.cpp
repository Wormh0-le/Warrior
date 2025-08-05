// Advanced Action RPG Learning, Wormhole All Rights Reserved

#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"

#include "Items/Weapons/WarriorHeroWeapon.h"


AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UHeroCombatComponent::OnWeaponHitTargetActor(AActor* TargetActor)
{
	if (OverlappedActors.Contains(TargetActor))	return;
	OverlappedActors.AddUnique(TargetActor);

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = TargetActor;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Shared_Event_MeleeHit,
		EventData
	);
}

void UHeroCombatComponent::OnWeaponPullback(AActor* TargetActor)
{
}
