// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartupDataBase.h"
#include "DataAsset_HeroStartupData.generated.h"

struct FWarriorHeroAbilitySet;
/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_HeroStartupData : public UDataAsset_StartupDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData", meta = (TitleProperty="InputTag"))
	TArray<FWarriorHeroAbilitySet> HeroStartUpAbilitySets;
};
