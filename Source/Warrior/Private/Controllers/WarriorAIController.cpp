// Advanced Action RPG Learning, Wormhole All Rights Reserved


#include "Controllers/WarriorAIController.h"

#include "StateTreeSchema.h"
#include "WarriorGameplayTags.h"
#include "Components/StateTreeAIComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());

	EnemySenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfigSight");
	EnemySenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	EnemySenseConfigSight->DetectionByAffiliation.bDetectFriendlies = false;
	EnemySenseConfigSight->DetectionByAffiliation.bDetectNeutrals = false;
	EnemySenseConfigSight->SightRadius = 5000.f;
	EnemySenseConfigSight->LoseSightRadius = 0.f;
	EnemySenseConfigSight->PeripheralVisionAngleDegrees = 360.f;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*EnemySenseConfigSight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));

	// EnemyStateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>("EnemyStateTreeAIComponent");
	// EnemyStateTreeAIComponent->SetStartLogicAutomatically(false);
}

void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();
	// if (EnemyStateTreeAIComponent->GetSchema())
	// {
	// 	EnemyStateTreeAIComponent->StartLogic();
	// }
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;
		default:
			break;
		}
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		// if (EnemyStateTreeAIComponent->GetSchema())
		// {
		// 	FStateTreeEvent Event;
		// 	Event.Tag = WarriorGameplayTags::Enemy_Status_Alert;
		// 	Event.Payload = FInstancedStruct::Make(FVector(Actor->GetActorLocation()));
		// 	EnemyStateTreeAIComponent->SendStateTreeEvent(Event);	
		// }
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
}
