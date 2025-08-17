// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "WarriorTypes/WarriorCountdownAction.h"

void FWarriorCountdownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountdownOutput = EWarriorCountdownActionOutput::Canceled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	if (ElapsedTimeSinceStart >= TotalCountdownTime)
	{
		CountdownOutput = EWarriorCountdownActionOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	} else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();
		OutRemainingTime = TotalCountdownTime - ElapsedTimeSinceStart;
		CountdownOutput = EWarriorCountdownActionOutput::Updated;
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		ElapsedInterval = 0.f;
	}
}

void FWarriorCountdownAction::CancelAction()
{
	bNeedToCancel = true;
}
