// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"


UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility,
	FGameplayTag EventTag, TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* MyObj = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
	MyObj->EventTag = EventTag;
	MyObj->SoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	MyObj->RandomSpawnRadius = RandomSpawnRadius;
	MyObj->NumToSpawn = NumToSpawn;
	
	return MyObj;
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	TaskDelegateHandle = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(EventTag).AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	if (TaskDelegateHandle.IsValid())
	{
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(EventTag).Remove(TaskDelegateHandle);	
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* Payload)
{
	if (ensure(!SoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnSoftEnemyLoaded)
		);
	} else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnSpawnFailed.Broadcast(TArray<AWarriorEnemyCharacter*>{});
		}
		EndTask();	
	}
}

void UAbilityTask_WaitSpawnEnemies::OnSoftEnemyLoaded()
{
	UClass* LoadedClass = SoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	TArray<AWarriorEnemyCharacter*> SpawnedEnemies{};
	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnSpawnFailed.Broadcast(SpawnedEnemies);
		}
		EndTask();
		return;
	}
	float HalfHeight = 150.f;
	const AWarriorEnemyCharacter* EnemyClass = SoftEnemyClassToSpawn->GetDefaultObject<AWarriorEnemyCharacter>();
	if (EnemyClass)
	{
		HalfHeight = EnemyClass->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnOrigin = GetAvatarActor()->GetActorLocation();
	FRotator SpawnRotator = GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();
	
	for (int32 i = 0; i < NumToSpawn; i++)
	{
		FVector SpawnLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, SpawnOrigin, SpawnLocation, RandomSpawnRadius);
		SpawnLocation += FVector(0.f, 0.f, HalfHeight);
		AWarriorEnemyCharacter* SpawnedEnemy = World->SpawnActor<AWarriorEnemyCharacter>(
			LoadedClass,
			SpawnLocation,
			SpawnRotator,
			SpawnParams
		);
		
		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (SpawnedEnemies.Num() > 0)
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		} else
		{
			OnSpawnFailed.Broadcast(SpawnedEnemies);
		}
	}
	EndTask();
}
