// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "BehaviorTree/Service_OrientToTarget.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

#include "Kismet/KismetMathLibrary.h"


UService_OrientToTarget::UService_OrientToTarget()
{
	NodeName = TEXT("Native Orient To Target");
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationInterpSpeed = 5.0f;
	Interval = 0.f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UService_OrientToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UService_OrientToTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient to %s key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UService_OrientToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
		OwningPawn->SetActorRotation(TargetRot);
	}
}
