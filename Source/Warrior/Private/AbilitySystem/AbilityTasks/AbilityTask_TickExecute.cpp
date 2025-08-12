// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilitySystem/AbilityTasks/AbilityTask_TickExecute.h"


UAbilityTask_TickExecute::UAbilityTask_TickExecute()
{
	bTickingTask = true;
}

UAbilityTask_TickExecute* UAbilityTask_TickExecute::TickExecute(UGameplayAbility* OwningAbility)
{
	UAbilityTask_TickExecute* MyObj = NewAbilityTask<UAbilityTask_TickExecute>(OwningAbility);
	return MyObj;
}

void UAbilityTask_TickExecute::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	} else
	{
		EndTask();
	}
}
