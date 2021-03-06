// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ProjectSticky/Interfaces/HealthManipulation.h"
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

	virtual void Tick(float DeltaSeconds) override;

	// Time taken for the ability to fully charge
	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	float fullChargeTime = 0.5;
	float currentChargeTime = 0;

	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	float baseRange = 100;

	// Basic function
	UFUNCTION(Category = "Ability")
	virtual void ChargeAbility(AActor* user, FVector direction, FVector mouseLocation);
	// Server functions
	UFUNCTION(Server, Reliable, WithValidation, Category = "Ability")
	virtual void Server_ChargeAbility(AActor* user, FVector direction, FVector mouseLocation);
	UFUNCTION(NetMulticast, Reliable, Category = "Ability")
	virtual void Multi_ChargeAbility(AActor* user, FVector direction, FVector mouseLocation);

	// Basic function 
	UFUNCTION(Category = "Ability")
	virtual void ExecuteAbility(AActor* user, FVector direction, FVector mouseLocation);
	// Server functions
	UFUNCTION(Server, Reliable, WithValidation, Category = "Ability")
	virtual void Server_ExecuteAbility(AActor* user, FVector direction, FVector mouseLocation);
	UFUNCTION(NetMulticast, Reliable, Category = "Ability")
	virtual void Multi_ExecuteAbility(AActor* user, FVector direction, FVector mouseLocation);
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnParticleEffect(UParticleSystem* particleSystem, FTransform worldTransform);
	UFUNCTION()
	UParticleSystemComponent* SpawnLastingParticleEffect(UParticleSystem* particleSystem, FTransform worldTransform);


	UPROPERTY(EditAnywhere, Category = "Damage")
	TArray<FDamageStruct> abilityDamage;

	// Base damage for the ability
	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	TArray<FDamage> AbilityBaseDamage;

	// Generic damage modifier
	UPROPERTY(EditAnywhere, Category = "Ability Stats")
	float overallDamageMod = 1;

	UPROPERTY()
	bool isCharging = false;
	UPROPERTY()
	AActor* currentUser;
	UPROPERTY()
	bool IsDebugActive = false;

	// These are the base VFX systems most effects will make use of.
	// What these systems are depends on ability, will be set in BP child classess
	UPROPERTY(EditAnywhere, Category = "AbilityVFX")
	UParticleSystem* PS_AbilityCharge;
	UPROPERTY(EditAnywhere, Category = "AbilityVFX")
	UParticleSystem* PS_AbilityChargeLoop;
	UPROPERTY(EditAnywhere, Category = "AbilityVFX")
	UParticleSystem* PS_AbilityExecutionPE;
	UPROPERTY(EditAnywhere, Category = "AbilityVFX")
	UParticleSystem* PS_AbilityHitPE;

	UPROPERTY()
	UParticleSystemComponent* chargeParticleSystem;
	UPROPERTY()
	UParticleSystemComponent* chargeParticleSystemLoop;

	// Called when actually hitting another actor
	UFUNCTION()
	bool DamageActor(AActor* actor);

	// Basic hit detection functions
	UFUNCTION()
	bool Damage_BoxTrace(FVector location, FVector boxDimensions, TArray<AActor*> actorsToIgnore);
	UFUNCTION()
	bool Damage_SphereTrace(FVector location, float radius, TArray<AActor*> actorsToIgnore);

public:

	UFUNCTION()
	void SetCurrentUser(AActor* user);
};
