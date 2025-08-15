// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Items/WarriorProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Sound/SoundCue.h"


// Sets default values
AWarriorProjectile::AWarriorProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);

	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	ProjectileNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComp"));
	ProjectileNiagaraComp->SetupAttachment(GetRootComponent());

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 700.f;
	ProjectileMovementComp->MaxSpeed = 1200.f;
	ProjectileMovementComp->Velocity = FVector(1.2f, 0.f, 0.f);
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

void AWarriorProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	if (ProjectileSpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ProjectileSpawnSound,
			GetActorLocation()
		);	
	}
	if (ProjectileMuzzleFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ProjectileMuzzleFX,
			GetActorLocation(),
			UKismetMathLibrary::MakeRotFromX(GetActorForwardVector())
		);
	}
	if (ProjectileFlyLoopSound && FlyLoopingSoundAttenuation && FlyLoopingSoundConcurrency)
	{
		ProjectileAudioComponent = UGameplayStatics::SpawnSoundAttached(
			ProjectileFlyLoopSound,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			UKismetMathLibrary::MakeRotFromX(GetActorForwardVector()),
			EAttachLocation::KeepWorldPosition,
			false,
			1.f,
			1.f,
			0.f,
			FlyLoopingSoundAttenuation,
			FlyLoopingSoundConcurrency,
			false
		);
	}
}

void AWarriorProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn || !UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))	return;
	SpawnHitFXEffect();
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHasTag(HitPawn, WarriorGameplayTags::Player_Status_Blocking);
	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetInstigator(), HitPawn);
	}
	FGameplayEventData EventData;
	EventData.Instigator = GetInstigator();
	EventData.Target = HitPawn;
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			EventData
		);
	}
	else
	{
		ApplyProjectileDamage(HitPawn, EventData);
	}
	Destroy();
}

void AWarriorProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedActors.Contains(OtherActor))	return;
	OverlappedActors.Add(OtherActor);
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UWarriorFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
		{
			FGameplayEventData EventData;
			EventData.Instigator = GetInstigator();
			EventData.Target = HitPawn;
			ApplyProjectileDamage(HitPawn, EventData);
		}
	}
}

void AWarriorProjectile::SpawnHitFXEffect()
{
	if (ProjectileAudioComponent && ProjectileAudioComponent->IsPlaying()){
		ProjectileAudioComponent->Stop();
	}
	if (ProjectileImpactSystem)
	{
		ProjectileImpactComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ProjectileImpactSystem,
			GetActorLocation(),
			UKismetMathLibrary::Conv_VectorToRotator(GetActorForwardVector())
		);
	}
	if (ProjectileImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ProjectileImpactSound,
			GetActorLocation()
		);	
	}
}

void AWarriorProjectile::ApplyProjectileDamage(APawn* HitPawn, const FGameplayEventData& InPayload)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign a valid spec handle to the projectile: %s"), *GetActorNameOrLabel());
	const bool bWasApplied = UWarriorFunctionLibrary::ApplayGameplayEffectSpecHandleToTarget(GetInstigator(), HitPawn, ProjectileDamageEffectSpecHandle);
	if (bWasApplied)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			WarriorGameplayTags::Shared_Event_HitReact,
			InPayload
		);
	}
}



