// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "PointAndClickAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API APointAndClickAbility : public ABaseAbility
{
	GENERATED_BODY()
	
public:

	APointAndClickAbility();

	virtual void ChargeAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	virtual void ServerChargeAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;

	virtual void ExecuteAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	virtual void ServerExecuteAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	
protected:

	UPROPERTY()
	FVector startPos;
	UPROPERTY()
	UParticleSystemComponent* chargeParticleSystem;

};
