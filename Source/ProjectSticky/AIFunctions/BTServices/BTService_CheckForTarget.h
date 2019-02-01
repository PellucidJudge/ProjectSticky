// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API UBTService_CheckForTarget : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_CheckForTarget();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
