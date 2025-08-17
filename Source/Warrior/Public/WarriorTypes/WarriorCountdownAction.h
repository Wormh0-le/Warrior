// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "WarriorEnumTypes.h"

/**
 * 
 */
class FWarriorCountdownAction : public  FPendingLatentAction
{
public:
	FWarriorCountdownAction(float InTotalCountdownTime, float InUpdateInterval, float& InOutRemainingTime, EWarriorCountdownActionOutput& InCountdownOutput, FLatentActionInfo& LatentActionInfo)
		:bNeedToCancel(false), TotalCountdownTime(InTotalCountdownTime), UpdateInterval(InUpdateInterval), OutRemainingTime(InOutRemainingTime),
		CountdownOutput(InCountdownOutput), ExecutionFunction(LatentActionInfo.ExecutionFunction), OutputLink(LatentActionInfo.Linkage),
		CallbackTarget(LatentActionInfo.CallbackTarget), ElapsedInterval(0.f), ElapsedTimeSinceStart(0.f)
	{
	};
	virtual void UpdateOperation(FLatentResponse& Response) override;
	
	void CancelAction();
private:
	bool bNeedToCancel;
	float TotalCountdownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EWarriorCountdownActionOutput& CountdownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};
