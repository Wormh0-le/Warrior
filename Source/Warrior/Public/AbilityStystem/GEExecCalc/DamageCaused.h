// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageCaused.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UDamageCaused : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UDamageCaused();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override; 
};
