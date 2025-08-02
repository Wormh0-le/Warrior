// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "WarriorWeaponBase.h"
#include "WarriorHeroWeapon.generated.h"

UCLASS()
class WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarriorHeroWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
