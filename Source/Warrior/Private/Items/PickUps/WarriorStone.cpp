// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Items/PickUps/WarriorStone.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Characters/WarriorHeroCharacter.h"

#include "Kismet/GameplayStatics.h"

#include "Sound/SoundCue.h"


// Sets default values
AWarriorStone::AWarriorStone()
{
	StoneNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StoneNiagaraComponent"));
	StoneNiagaraComponent->SetupAttachment(GetRootComponent());
}

void AWarriorStone::OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWarriorHeroCharacter* OverlappedHeroCharacter = Cast<AWarriorHeroCharacter>(OtherActor))
	{
		OverlappedHeroCharacter->GetWarriorAbilitySystemComponent()->TryActivateAbilityByTag(WarriorGameplayTags::Player_Ability_PickUp_Stone);
	}
}

void AWarriorStone::Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 AbilityLevel)
{
	check(StoneGameplayEffectClass);
	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		AbilityLevel,
		AbilitySystemComponent->MakeEffectContext()
	);
	Destroy();
}

void AWarriorStone::Destroyed()
{
	Super::Destroyed();
	if (StonePickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			StonePickUpEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	if (StonePickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			StonePickUpSound,
			GetActorLocation()
		);
	}
}


