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

	virtual void ChargeAbility_Implementation(AActor* user, FVector direction, float range) override;
	virtual void ServerChargeAbility_Implementation(AActor* user, FVector direction, float range) override;

	virtual void ExecuteAbility_Implementation(AActor* user, FVector direction, float range) override;
	virtual void ServerExecuteAbility_Implementation(AActor* user, FVector direction, float range) override;


protected:

	//UFUNCTION()
	//bool DamageActor(AActor * actor);
};
