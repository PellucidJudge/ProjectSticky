// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "MeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API AMeleeAbility : public ABaseAbility
{
	GENERATED_BODY()
	
public:

	void ChargeAbility(ABaseCharacter* user, FVector direction, float range);
	void ExecuteAbility(ABaseCharacter* user, FVector direction, float range);
	
};
