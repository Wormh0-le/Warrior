// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "AbilitySystem/Abilities/HeroAbility_ToggleTargetLock.h"

#include "EnhancedInputSubsystems.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WarriorWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "WarriorDebugHelper.h"


void UHeroAbility_ToggleTargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroAbility_ToggleTargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanUp();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_ToggleTargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();
	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetLockTargetFromAvailableActors(ActorsOnLeft);
	}
	else if (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Right)
	{
		NewTargetToLock = GetLockTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UHeroAbility_ToggleTargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();
	if(AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}
	CurrentLockedActor = GetLockTargetFromAvailableActors(AvailableActorsToLock);
	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();
		SetTargetLockWidgetPosition();
	} else
	{
		CancelTargetLockAbility();
	}
}

void UHeroAbility_ToggleTargetLock::GetAvailableActorsToLock()
{
	const AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
	TArray<FHitResult> BoxHitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		HeroCharacter,
		HeroCharacter->GetActorLocation(),
		HeroCharacter->GetActorLocation() + HeroCharacter->GetActorForwardVector() * BoxTraceDistance,
		BoxTraceExtent,
		HeroCharacter->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxHitResults,
		true
	);
	for (const FHitResult& TraceHit : BoxHitResults)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor->Implements<UPawnCombatInterface>())
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}		
		}
	}
}

void UHeroAbility_ToggleTargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft,
	TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}
	const FVector playerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerCurrentNormalized = (CurrentLockedActor->GetActorLocation() - playerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor)	continue;
		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - playerLocation).GetSafeNormal();
		const FVector CrossProduct = FVector::CrossProduct(PlayerCurrentNormalized, PlayerToAvailableNormalized);
		if (CrossProduct.Z > 0)
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		} else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}

void UHeroAbility_ToggleTargetLock::DrawTargetLockWidget()
{
	if (!TargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in blueprint"));
		AWarriorHeroController* HeroController = GetHeroControllerFromActorInfo();
		TargetLockWidget = CreateWidget<UWarriorWidget>(HeroController, TargetLockWidgetClass);
		check(TargetLockWidget);
		TargetLockWidget->AddToViewport();	
	}
}

void UHeroAbility_ToggleTargetLock::SetTargetLockWidgetPosition()
{
	if (!TargetLockWidget || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}
	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
	);
	if (TargetWidgetSize == FVector2D::ZeroVector)
	{
		TargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* Widget)
			{
				if (USizeBox* SizeBox = Cast<USizeBox>(Widget))
				{
					TargetWidgetSize.Y = SizeBox->GetHeightOverride();
					TargetWidgetSize.X = SizeBox->GetWidthOverride();
				}
			}
		);	
	}
	ScreenPosition -= TargetWidgetSize * .5f; // Center the widget on the target actor
	TargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroAbility_ToggleTargetLock::OnTargetLockTick(float DeltaTime)
{
	AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
	if (!HeroCharacter || !CurrentLockedActor ||
		UWarriorFunctionLibrary::NativeDoesActorHasTag(HeroCharacter, WarriorGameplayTags::Shared_Status_Dead) ||
		UWarriorFunctionLibrary::NativeDoesActorHasTag(CurrentLockedActor, WarriorGameplayTags::Shared_Status_Dead))
	{
		CancelTargetLockAbility();
		return;
	}
	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation = !UWarriorFunctionLibrary::NativeDoesActorHasTag(HeroCharacter, WarriorGameplayTags::Player_Status_Rolling) &&
		!UWarriorFunctionLibrary::NativeDoesActorHasTag(HeroCharacter, WarriorGameplayTags::Player_Status_Blocking);
	if (bShouldOverrideRotation)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			HeroCharacter->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);
		LookAtRot -= FRotator(TargetLockCameraOffset, 0.f, 0.f); 
		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		FRotator TargetControlRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);
		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetControlRot.Pitch, TargetControlRot.Yaw, CurrentControlRot.Roll));
		HeroCharacter->SetActorRotation(FRotator(0.f, TargetControlRot.Yaw, 0.f));
	}
}

AActor* UHeroAbility_ToggleTargetLock::GetLockTargetFromAvailableActors(const TArray<AActor*>& InAvailable)
{
	const AWarriorHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo();
	float ClosestDistance;
	return UGameplayStatics::FindNearestActor(HeroCharacter->GetActorLocation(), InAvailable, ClosestDistance);
}

void UHeroAbility_ToggleTargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroAbility_ToggleTargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;
	if (TargetLockWidget)
	{
		TargetLockWidget->RemoveFromParent();
		TargetLockWidget = nullptr;
		TargetWidgetSize = FVector2D::ZeroVector;
	}
}

void UHeroAbility_ToggleTargetLock::InitTargetLockMovement()
{
	DefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMovementMaxWalkSpeed;
}

void UHeroAbility_ToggleTargetLock::ResetTargetLockMovement()
{
	if (DefaultMaxWalkSpeed > 0.f)
	{
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;	
	}
	DefaultMaxWalkSpeed = 0.f;
}

void UHeroAbility_ToggleTargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	Subsystem->AddMappingContext(TargetLockInputMappingContext, 2);
}

void UHeroAbility_ToggleTargetLock::ResetTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo())	return;
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetLockInputMappingContext);
}
