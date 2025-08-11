// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "WarriorFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorGameplayTags.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"


UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	check(InActor);
	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UWarriorFunctionLibrary::NativeDoesActorHasTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWarriorFunctionLibrary::BP_DoesActorHasTag(AActor* InActor, FGameplayTag TagToCheck,
	EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHasTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = CombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;
	return CombatComponent;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}

FGameplayTag UWarriorFunctionLibrary::ComputeHitReactionDirectionTag(AActor* Attacker, AActor* Victim, float& OutAngleDegrees)
{
	check(Attacker && Victim);
	const FVector VictimFrontNormalized = Victim->GetActorForwardVector().GetSafeNormal();
	const FVector VictimToAttackerNormalized = (Attacker->GetActorLocation() - Victim->GetActorLocation()).GetSafeNormal();
	const float DotProduct = FVector::DotProduct(VictimFrontNormalized, VictimToAttackerNormalized);
	OutAngleDegrees = FMath::Acos(DotProduct) * (180.f / PI);
	const FVector CrossProduct = FVector::CrossProduct(VictimFrontNormalized, VictimToAttackerNormalized);
	if (CrossProduct.Z < 0.f)
	{
		OutAngleDegrees *= -1.f; 
	}
	FGameplayTag HitReactionDirectionTag = WarriorGameplayTags::Shared_Status_HitReact_Front;
	if (OutAngleDegrees >= -45.f && OutAngleDegrees < 45.f)
	{
		HitReactionDirectionTag = WarriorGameplayTags::Shared_Status_HitReact_Front;
	} else if (OutAngleDegrees >= 45.f && OutAngleDegrees < 135.f)
	{
		HitReactionDirectionTag = WarriorGameplayTags::Shared_Status_HitReact_Right;
	} else if (OutAngleDegrees >= -135.f && OutAngleDegrees < -45.f)
	{ 
		HitReactionDirectionTag = WarriorGameplayTags::Shared_Status_HitReact_Left;
	} else if (OutAngleDegrees >= 135.f || OutAngleDegrees < -135.f)
	{
		HitReactionDirectionTag = WarriorGameplayTags::Shared_Status_HitReact_Back;
	}
	return HitReactionDirectionTag;
}

bool UWarriorFunctionLibrary::IsValidBlock(AActor* Attacker, AActor* Defender)
{
	check(Attacker && Defender);

	const float DotResult = FVector::DotProduct(Attacker->GetActorForwardVector(), Defender->GetActorForwardVector());

	return DotResult < -.1f;
}



