// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilityStystem/Abilities/GAHeroToggleTargetLock.h"

#include "Characters/WarriorHeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WarriorDebugHelper.h"

void UGAHeroToggleTargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	TryLockOnTarget();
}

void UGAHeroToggleTargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGAHeroToggleTargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();
}

void UGAHeroToggleTargetLock::GetAvailableActorsToLock()
{
	AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
	TArray<FHitResult> BoxHitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		HeroCharacter,
		HeroCharacter->GetActorLocation(),
		HeroCharacter->GetActorLocation() + HeroCharacter->GetActorForwardVector() * BoxTraceDistance,
		BoxTraceExtent,
		HeroCharacter->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxHitResults,
		true
	);
	for (const FHitResult& TraceHit : BoxHitResults)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor->Implements<UPawnCombatInterface>())
			{
				AvailableActorsToLock.AddUnique(HitActor);
				Debug::Print(TEXT("Found Actor to Lock: ") + HitActor->GetName());
			}		
		}
	}
}
