// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorFunctionLibrary.generated.h"

class UWarriorGameInstance;
struct FGameplayEffectSpecHandle;
class UPawnCombatComponent;
struct FGameplayTag;
class UWarriorAbilitySystemComponent;


/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior| FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Warrior| FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHasTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Warrior| FunctionLibrary", meta = (DisplayName = "Does Actor Has Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHasTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);
	
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior| FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "Warrior| FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Warrior| FunctionLibrary")
	static FGameplayTag ComputeHitReactionDirectionTag(AActor* Attacker, AActor* Victim, float& OutAngleDegrees);

	UFUNCTION(BlueprintPure, Category = "Warrior| FunctionLibrary")
	static bool IsValidBlock(AActor* Attacker, AActor* Defender);
	
	UFUNCTION(BlueprintCallable, Category = "Warrior| FunctionLibrary")
	static bool ApplayGameplayEffectSpecHandleToTarget(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Warrior| FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentActionInfo", ExpandEnumAsExecs = "CountdownInput|CountdownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void Countdown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,
		float& OutRemainingTime, EWarriorCountdownActionInput CountdownInput,
		UPARAM(DisplayName = "Output") EWarriorCountdownActionOutput& CountdownOutput, FLatentActionInfo LatentActionInfo
	);

	UFUNCTION(BlueprintPure, Category = "Warrior| FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UWarriorGameInstance* GetWarriorGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Warrior| FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, EWarriorInputMode InputMode);
	
};
