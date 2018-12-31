// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthManipulation.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealthManipulation : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTSTICKY_API IHealthManipulation
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void DamageObject(float damage, AActor* attacker, float knockBackDistance, FVector knockBackDir);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void KillObject();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Healing")
	void HealObject(float healing, AActor* healer);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Healing")
	void FullHealObject();
};
