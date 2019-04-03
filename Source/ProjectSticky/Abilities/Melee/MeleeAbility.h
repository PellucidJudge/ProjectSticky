// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "ProjectSticky/Utility/Structs/HealthManipulationStructs.h"
#include "MeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API AMeleeAbility : public ABaseAbility
{
	GENERATED_BODY()
	
public:

	AMeleeAbility();

	virtual void Tick(float DeltaSeconds) override;

	virtual void Server_ChargeAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	virtual void Multi_ChargeAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;

	virtual void Server_ExecuteAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	virtual void Multi_ExecuteAbility_Implementation(AActor* user, FVector direction, FVector mouseLocation) override;
	
private:

	virtual void BeginPlay() override;

	UPROPERTY()
	UParticleSystemComponent* PSC_ChargeParticles;

	//UFUNCTION()
	//bool DamageActor(AActor * actor);
};
