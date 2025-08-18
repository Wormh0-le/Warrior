// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "WarriorPickUp.h"
#include "WarriorStone.generated.h"

class UNiagaraSystem;
class UGameplayEffect;
class UWarriorAbilitySystemComponent;
class UNiagaraComponent;

UCLASS()
class WARRIOR_API AWarriorStone : public AWarriorPickUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarriorStone();

	void Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 AbilityLevel);

	virtual void Destroyed() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* StoneNiagaraComponent;

	virtual void OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* StonePickUpEffect;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* StonePickUpSound;
};
