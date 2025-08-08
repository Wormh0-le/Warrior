// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_OrientToTarget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UService_OrientToTarget : public UBTService
{
	GENERATED_BODY()
	
	UService_OrientToTarget();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed;

	
};
