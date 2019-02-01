// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"

ABaseAIController::ABaseAIController()
{
	// Create the blackboard and behavior components
	blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// Get controlled character reference
	APawn* controlledPawn = InPawn;
	controlledCharRef = Cast<ABaseAICharacter>(controlledPawn);

	if (controlledCharRef && controlledCharRef->AIBehavior)
	{

		blackboardComponent->InitializeBlackboard(*controlledCharRef->AIBehavior->BlackboardAsset);

		targetKeyID = blackboardComponent->GetKeyID("Target");
		homeLocationKeyID = blackboardComponent->GetKeyID("HomeLocation");
		wantedLocationKeyID = blackboardComponent->GetKeyID("WantedLocation");
		wantToAttackKeyID = blackboardComponent->GetKeyID("WantToAttack");

		behaviorTreeComponent->StartTree(*controlledCharRef->AIBehavior);
	}
}
