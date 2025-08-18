// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarriorBaseGameMode.generated.h"

enum class EWarriorGameDifficulty : uint8;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWarriorBaseGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EWarriorGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EWarriorGameDifficulty GetGameDifficulty() const { return CurrentGameDifficulty; }
};
