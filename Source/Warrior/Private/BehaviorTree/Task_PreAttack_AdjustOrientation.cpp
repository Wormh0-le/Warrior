// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "BehaviorTree/Task_PreAttack_AdjustOrientation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

#include "Kismet/KismetMathLibrary.h"


UTask_PreAttack_AdjustOrientation::UTask_PreAttack_AdjustOrientation()
{
	NodeName = TEXT("Native Pre Attack Adjust Orientation");
	AnglePrecision = 10.f;
	RotationInterpSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UTask_PreAttack_AdjustOrientation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UTask_PreAttack_AdjustOrientation::GetInstanceMemorySize() const
{
	return sizeof(FPreAttackTaskMemory);
}

FString UTask_PreAttack_AdjustOrientation::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Smoothly rotate to face %s before attack until reached the target angle.\n\n tolerance degrees:%.1f \n interpolation speed: %.1f"), *KeyDescription, AnglePrecision, RotationInterpSpeed);
}

EBTNodeResult::Type UTask_PreAttack_AdjustOrientation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FPreAttackTaskMemory* Memory = CastInstanceNodeMemory<FPreAttackTaskMemory>(NodeMemory);
	check(Memory);
	Memory->Reset();
	
	APawn* OwningPawn = AIController->GetPawn();
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	if (Memory->IsValid())
	{
		if (HasReachedTargetAngleWithTolerance(OwningPawn, TargetActor))
		{
			Result = EBTNodeResult::Succeeded;
		} else {
			Result = EBTNodeResult::InProgress;
		}
	}
	return Result;
}

void UTask_PreAttack_AdjustOrientation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	FPreAttackTaskMemory* Memory = CastInstanceNodeMemory<FPreAttackTaskMemory>(NodeMemory);
	if (!Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	if (HasReachedTargetAngleWithTolerance(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	} else
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
		Memory->OwningPawn->SetActorRotation(TargetRot);
	}
}

bool UTask_PreAttack_AdjustOrientation::HasReachedTargetAngleWithTolerance(APawn* QueryPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation());
	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);
	return AngleDiff <= AnglePrecision;
}
