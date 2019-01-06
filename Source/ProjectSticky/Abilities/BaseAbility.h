// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "BaseAbility.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDamage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageStruct")
	UDamageType* damageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageStruct")
	float BaseDamageAmount;
};

UCLASS()
class PROJECTSTICKY_API ABaseAbility : public AActor
{
	GENERATED_BODY()
	
public:

	ABaseAbility();

	// Time taken for the ability to fully charge
	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	float fullChargeTime = 0.5;

	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	float baseRange = 100;

	// Server side functions for 
	UFUNCTION(Server, Reliable, WithValidation, Category = "Ability")
	virtual void ChargeAbility(AActor* user, FVector direction, FVector mouseLocation);
	UFUNCTION(NetMulticast, Reliable, Category = "Ability")
	virtual void ServerChargeAbility(AActor* user, FVector direction, FVector mouseLocation);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Ability")
	virtual void ExecuteAbility(AActor* user, FVector direction, FVector mouseLocation);
	UFUNCTION(NetMulticast, Reliable, Category = "Ability")
	virtual void ServerExecuteAbility(AActor* user, FVector direction, FVector mouseLocation);
	
protected:

	virtual void BeginPlay() override;

	// Base damage for the ability
	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	TArray<FDamage> AbilityBaseDamage;

	// Generic damage modifier
	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	float overallDamageMod = 1;

	// These are the base VFX systems most effects will make use of.
	// What these systems are depends on ability, will be set in BP child classess
	UPROPERTY(EditAnywhere, Category = "AbilityVFX")
	UParticleSystem* PS_AbilityCharge;
	UPROPERTY(EditAnywhere, Category = "AbilityVFX")
	UParticleSystem* PS_AbilityExecutionPE;
	UPROPERTY(EditAnywhere, Category = "AbilityVFX")
	UParticleSystem* PS_AbilityHitPE;

	// Called when actually hitting another actor
	UFUNCTION()
	bool DamageActor(AActor* actor);

	// Basic hit detection functions
	//UFUNCTION()
	//	bool MultiBoxTrace(FHitResult* Hit, )
};
