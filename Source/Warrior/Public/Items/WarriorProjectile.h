// Advanced Action RPG Learning, Wormhole All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "GameFramework/Actor.h"
#include "WarriorProjectile.generated.h"

class UGameplayEffect;
struct FGameplayEventData;
class UNiagaraSystem;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap
};

UCLASS()
class WARRIOR_API AWarriorProjectile : public AActor
{
	GENERATED_BODY()

public:
	AWarriorProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnHitFXEffect();

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Impact")
	UNiagaraSystem* ProjectileImpactSystem;

	UPROPERTY()
	UNiagaraComponent* ProjectileImpactComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Impact")
	USoundCue* ProjectileSpawnSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Impact")
	USoundCue* ProjectileImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Fly")
	UNiagaraSystem* ProjectileMuzzleFX;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Fly")
	USoundCue* ProjectileFlyLoopSound;

	UPROPERTY()
	UAudioComponent* ProjectileAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Fly")
	USoundAttenuation* FlyLoopingSoundAttenuation;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Fly")
	USoundConcurrency* FlyLoopingSoundConcurrency;
	
	UPROPERTY(BlueprintReadOnly, Category = "Projectile|GameplayEffect", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;
private:
	void ApplyProjectileDamage(APawn* HitPawn, const FGameplayEventData& InPayload);
};
