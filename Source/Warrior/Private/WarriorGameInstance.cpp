// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "WarriorGameInstance.h"

#include "WarriorTypes/WarriorStructTypes.h"


TSoftObjectPtr<UWorld> UWarriorGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FWarriorGameLevelSet& LevelSet : GameLevelSets)
	{
		if (!LevelSet.IsValid())	continue;
		if (LevelSet.LevelTag == InTag)
		{
			return LevelSet.Level;
		}
	}
	return TSoftObjectPtr<UWorld>();
}
