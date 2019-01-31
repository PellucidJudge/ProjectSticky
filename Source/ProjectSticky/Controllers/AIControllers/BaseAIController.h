// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "ProjectSticky/Characters/BaseAICharacter.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ABaseAIController();

	virtual void Tick(float DeltaSeconds) override;

	virtual void Possess(APawn* InPawn) override;

private:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	ABaseAICharacter* controlledCharRef;
	UPROPERTY(transient)
	UBlackboardComponent* blackboardComponent;
	UPROPERTY(transient)
	UBehaviorTreeComponent* behaviorTreeComponent;

	uint8 targetKeyID;
	uint8 homeLocationKeyID;
	uint8 wantedLocationKeyID;

	uint8 wantToAttackKeyID;
};
