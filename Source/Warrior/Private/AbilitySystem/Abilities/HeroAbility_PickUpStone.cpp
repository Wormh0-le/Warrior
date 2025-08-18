// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilitySystem/Abilities/HeroAbility_PickUpStone.h"

#include "Characters/WarriorHeroCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Items/PickUps/WarriorStone.h"
#include "Kismet/KismetSystemLibrary.h"


void UHeroAbility_PickUpStone::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroAbility_PickUpStone::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_PickUpStone::CollectStones()
{
	CollectedStones.Empty();
	const AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
	const FVector Down = -HeroCharacter->GetActorUpVector();
	const FVector Start = HeroCharacter->GetActorLocation();
	const FVector End = Start + Down * BoxTraceDistance;
	const EDrawDebugTrace::Type DrawDebugType = bDrawDebugShape ? EDrawDebugTrace::ForOneFrame :EDrawDebugTrace::None;

	TArray<FHitResult> TraceHits;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		HeroCharacter,
		Start,
		End,
		TraceBoxSize / 2.f,
		Down.ToOrientationRotator(),
		StoneTraceChannel,
		false,
		ActorsToIgnore,
		DrawDebugType,
		TraceHits,
		true
	);

	for (const FHitResult& Hit : TraceHits)
	{
		if (AWarriorStone* FoundStone = Cast<AWarriorStone>(Hit.GetActor()))
		{
			CollectedStones.Add(FoundStone);
		}
	}
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UHeroAbility_PickUpStone::ConsumeStones()
{
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
	for (AWarriorStone* Stone : CollectedStones)
	{
		if (Stone)
		{
			Stone->Consume(GetWarriorAbilitySystemComponentFromActorInfo(), GetAbilityLevel());	
		}
	}
}
