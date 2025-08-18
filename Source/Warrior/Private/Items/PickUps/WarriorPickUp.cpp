// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Items/PickUps/WarriorPickUp.h"

#include "Components/SphereComponent.h"


// Sets default values
AWarriorPickUp::AWarriorPickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
	SetRootComponent(OverlapSphere);
	OverlapSphere->InitSphereRadius(50.f);
	OverlapSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnOverlapSphereBeginOverlap);
}

void AWarriorPickUp::OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

