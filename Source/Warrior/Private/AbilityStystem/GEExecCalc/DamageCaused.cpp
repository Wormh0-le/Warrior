// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilityStystem/GEExecCalc/DamageCaused.h"

#include "WarriorDebugHelper.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAttributeSet.h"


struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefencePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(OutputDamage)

	FWarriorDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefencePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, OutputDamage, Target, false);
	}
};

static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
	static FWarriorDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}

UDamageCaused::UDamageCaused()
{
	// Slow way of doing capture
	// FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	// 	UWarriorAttributeSet::StaticClass(),
	// 	GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet, AttackPower)
	// );
	// FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDef(
	// 	AttackPowerProperty,
	// 	EGameplayEffectAttributeCaptureSource::Source,
	// 	false
	// );
	// RelevantAttributesToCapture.Add(AttackPowerCaptureDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefencePowerDef);
}

void UDamageCaused::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	// EffectSpec.GetContext().GetSourceObject();
	// EffectSpec.GetContext().GetAbility();
	// EffectSpec.GetContext().GetInstigator();
	// EffectSpec.GetContext().GetEffectCauser();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetWarriorDamageCapture().AttackPowerDef,
		EvaluateParameters,
		SourceAttackPower
	);
	Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float BaseDamage = 0.f;
	int32 LightAttackComboCount = 0;
	int32 HeavyAttackComboCount = 0;
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			LightAttackComboCount = TagMagnitude.Value;
			Debug::Print(TEXT("LightAttackComboCount"), LightAttackComboCount);
		}
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			HeavyAttackComboCount = TagMagnitude.Value;
			Debug::Print(TEXT("HeavyAttackComboCount"), HeavyAttackComboCount);
		}
	}

	float TargetDefencePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetWarriorDamageCapture().DefencePowerDef,
		EvaluateParameters,
		TargetDefencePower
	);
	Debug::Print(TEXT("TargetDefencePower"), TargetDefencePower);
	
	const float DamageIncreasePercentLight = LightAttackComboCount * 0.05f + 1.f;
	BaseDamage *= DamageIncreasePercentLight;
	Debug::Print(TEXT("DamageIncreasePercentLight"), DamageIncreasePercentLight);
	
	const float DamageIncreasePercentHeavy = HeavyAttackComboCount * 0.15f + 1.12f;
	BaseDamage *= DamageIncreasePercentHeavy;
	Debug::Print(TEXT("DamageIncreasePercentHeavy"), DamageIncreasePercentHeavy);
	
	const float FinalDamage = BaseDamage * SourceAttackPower / TargetDefencePower;
	Debug::Print(TEXT("FinalDamage"), FinalDamage);
	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().OutputDamageProperty,
				EGameplayModOp::Override,
				FinalDamage
			)
		);
	}
}
