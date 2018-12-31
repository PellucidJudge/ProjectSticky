// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "ProjectSticky/Characters/BaseCharacter.h"
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

	void ChargeAbility(ABaseCharacter* user, FVector direction);
	void ExecuteAbility(ABaseCharacter* user, FVector direction);
	
protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = "AbilityDamage")
	TArray<FDamage> AbilityBaseDamage;

	UPROPERTY(EditAnywhere, Category = "AbilityDamage")
	float overallDamageMod = 1;

};
