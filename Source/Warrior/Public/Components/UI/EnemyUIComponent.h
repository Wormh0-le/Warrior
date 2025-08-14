// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UWarriorWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARRIOR_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void RegisterWidget(UWarriorWidget* InWidgetToRegister);

	UFUNCTION(BlueprintCallable)
	void UnregisterEnemyOwnedWidgets();
private:
	UPROPERTY()
	TArray<UWarriorWidget*> EnemyOwnedWidgets;
};
