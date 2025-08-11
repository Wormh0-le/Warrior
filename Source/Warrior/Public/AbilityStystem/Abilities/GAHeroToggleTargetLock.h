// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "GAHeroToggleTargetLock.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UGAHeroToggleTargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()
protected:

	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector BoxTraceExtent = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistentDebugShape = false;

	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;
};
