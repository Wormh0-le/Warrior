// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "GameModes/WarriorSurvivalGameMode.h"

#include "NavigationSystem.h"

#include "Characters/WarriorEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"

#include "Kismet/GameplayStatics.h"

#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorTypes/WarriorStructTypes.h"


void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table in survival game mode blueprint"));

	SetCurrentSurvivalGameState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
	PreloadNextWaveEnemies();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentSurvivalGameState == EWarriorSurvivalGameModeState::WaitSpawnNewWave) 
	{
		TimePassedSinceStart += DeltaTime;
		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.f;
			SetCurrentSurvivalGameState(EWarriorSurvivalGameModeState::SpawningNewWave);
		}
	}
	if (CurrentSurvivalGameState == EWarriorSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;
		if (TimePassedSinceStart >= SpawnEnemyDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
			TimePassedSinceStart = 0.f;
			SetCurrentSurvivalGameState(EWarriorSurvivalGameModeState::InProgress);
		}
	}
	if (CurrentSurvivalGameState == EWarriorSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;
		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.f;
			CurrentWave += 1;
			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameState(EWarriorSurvivalGameModeState::AllWavesDone);	
			} else
			{
				SetCurrentSurvivalGameState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
				PreloadNextWaveEnemies();
			}
		}
	}
}

void AWarriorSurvivalGameMode::SetCurrentSurvivalGameState(EWarriorSurvivalGameModeState InState)
{
	CurrentSurvivalGameState = InState;

	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameState);
}

bool AWarriorSurvivalGameMode::HasFinishedAllWaves()
{
	return CurrentWave > TotalWavesToSpawn;
}

bool AWarriorSurvivalGameMode::ShouldKeepSpawnEnemies()
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AWarriorSurvivalGameMode::PreloadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;
	}
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	for (FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull() || PreloadedEnemyClassMap.Contains(SpawnerInfo.SoftEnemyClassToSpawn))	continue;
		StreamableManager.RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreloadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					}
				}
			)
		);
	}
}

int32 AWarriorSurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);	
	}
	checkf(!TargetPointsArray.IsEmpty(), TEXT("No Valid Target Point found in level: %s for spawning enemies"), *GetWorld()->GetMapName());
	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())	continue;
		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);
		UClass* LoadedEnemyClass = PreloadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);
		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();
			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(
				this,
				SpawnOrigin,
				RandomLocation,
				400.f
			);
			RandomLocation += FVector(0.f, 0.f, 150.f);
			AWarriorEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AWarriorEnemyCharacter>(
				LoadedEnemyClass,
				RandomLocation,
				SpawnRotation,
				SpawnParameters
			);
			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnSpawnedEnemyDestroy);
				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}
			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}
	return EnemiesSpawnedThisTime;
}

void AWarriorSurvivalGameMode::OnSpawnedEnemyDestroy(AActor* DestroyedActor)
{
	CurrentSpawnedEnemiesCounter--;
	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	} else if (CurrentSpawnedEnemiesCounter == 0) {
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;
		
		SetCurrentSurvivalGameState(EWarriorSurvivalGameModeState::WaveCompleted);
	}
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWave));
	FWarriorEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RowName, FString{});
	checkf(FoundRow, TEXT("Could not find a valid row under the name %s in the data table"), *RowName.ToString());
	return FoundRow;
}

void AWarriorSurvivalGameMode::RegisterSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesTotRegister)
{
	for (AWarriorEnemyCharacter* SpawnedEnemy : InEnemiesTotRegister)
	{
		if (SpawnedEnemy)
		{
			CurrentSpawnedEnemiesCounter++;
			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnSpawnedEnemyDestroy);
		}
	}
}
