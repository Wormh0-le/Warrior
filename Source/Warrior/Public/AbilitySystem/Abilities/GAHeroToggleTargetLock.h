// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "GAHeroToggleTargetLock.generated.h"

class UInputMappingContext;
class UWarriorWidget;
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

	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
	
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);
private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();

	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	AActor* GetLockTargetFromAvailableActors(const TArray<AActor*>& InAvailable);

	void CancelTargetLockAbility();
	void CleanUp();

	void InitTargetLockMovement();
	void ResetTargetLockMovement();

	void InitTargetLockMappingContext();
	void ResetTargetLockMappingContext();

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector BoxTraceExtent = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistentDebugShape = false;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TSubclassOf<UWarriorWidget> TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockRotationInterpSpeed = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockCameraOffset = 20.f;

	UPROPERTY()
	FVector2D TargetWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	TObjectPtr<UWarriorWidget> TargetLockWidget;

	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;

	UPROPERTY()
	float DefaultMaxWalkSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockMovementMaxWalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	UInputMappingContext* TargetLockInputMappingContext;
};
