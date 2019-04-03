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

	virtual void Tick(float DeltaSeconds) override;

	virtual void Server_ChargeAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	virtual void Multi_ChargeAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;

	virtual void Server_ExecuteAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	virtual void Multi_ExecuteAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	FVector startPos;

	UPROPERTY()
	float psEndSize = 2;
	UPROPERTY()
	float psCurrentSizeProgression;

};
