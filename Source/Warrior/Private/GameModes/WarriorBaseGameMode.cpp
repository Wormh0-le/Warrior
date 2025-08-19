// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "GameModes/WarriorBaseGameMode.h"

#include "WarriorFunctionLibrary.h"

AWarriorBaseGameMode::AWarriorBaseGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AWarriorBaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	EWarriorGameDifficulty GameDifficulty;
	if (UWarriorFunctionLibrary::TryLoadSavedGameDifficulty(GameDifficulty))
	{
		CurrentGameDifficulty = GameDifficulty;
	}
	
}
