// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForTarget.h"
#include "ProjectSticky/Controllers/AIControllers/BaseAIController.h"
#include "Engine.h"


UBTService_CheckForTarget::UBTService_CheckForTarget()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForTarget::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			ABaseCharacter* target = Cast<ABaseCharacter>(world->GetFirstPlayerController()->GetPawn());

			if (target)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(AIController->targetKeyID, target);
			}
		}
		
	}
}
