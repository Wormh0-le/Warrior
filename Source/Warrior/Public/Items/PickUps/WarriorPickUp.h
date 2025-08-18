// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorPickUp.generated.h"

class USphereComponent;

UCLASS()
class WARRIOR_API AWarriorPickUp : public AActor
{
	GENERATED_BODY()

public:
	AWarriorPickUp();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickUp|Interaction")
	TObjectPtr<USphereComponent> OverlapSphere;

	UFUNCTION()
	virtual void OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
