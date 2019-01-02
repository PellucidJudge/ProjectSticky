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

	UFUNCTION(Server, Reliable, WithValidation, Category = "Ability")
	virtual void ChargeAbility(AActor* user, FVector direction, float range);
	UFUNCTION(NetMulticast, Reliable, Category = "Ability")
	virtual void ServerChargeAbility(AActor* user, FVector direction, float range);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Ability")
	virtual void ExecuteAbility(AActor* user, FVector direction, float range);
	UFUNCTION(NetMulticast, Reliable, Category = "Ability")
	virtual void ServerExecuteAbility(AActor* user, FVector direction, float range);
	
protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = "AbilityDamage")
	TArray<FDamage> AbilityBaseDamage;

	UPROPERTY(EditAnywhere, Category = "AbilityDamage")
	float overallDamageMod = 1;

	UFUNCTION()
	bool DamageActor(AActor* actor);

};
