#pragma once

UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EWarriorValidType: uint8
{
	Valid,
	Invalid,
};

UENUM()
enum class EWarriorSuccessType : uint8
{
	Success,
	Fail,
};

UENUM(BlueprintType)
enum class EWarriorDecoratorType : uint8
{
	IsLessThan,
	IsLessThanOrEqualTo,
	EqualTo,
	IsGreaterThanOrEqualTo,
	IsGreaterThan,
};

UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

UENUM(BlueprintType)
enum class EWarriorCountdownActionInput : uint8
{
	Start,
	Cancel
};

UENUM(BlueprintType)
enum class EWarriorCountdownActionOutput : uint8
{
	Updated,
	Completed,
	Canceled,
};

UENUM(BlueprintType)
enum class EWarriorSurvivalGameModeState: uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};