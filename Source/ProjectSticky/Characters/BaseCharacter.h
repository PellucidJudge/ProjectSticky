// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "ProjectSticky/Interfaces/HealthManipulation.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECTSTICKY_API ABaseCharacter : public ACharacter, public IHealthManipulation
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Character stats
	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float healthMax = 3;
	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float healthStart = 3;
	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float healthCurrent;
	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float CharDamage;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	bool IsChargingAnAttack = false;

	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float MovementSpeed = 600;
	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float chargingMovePenalty = 0.5;

public:	

	UPROPERTY()
	int32 numberOfUsedAttackSlots;
	
	// Health manipulation interface functions
	UFUNCTION(BLueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void DamageObject(float damage, AActor* attacker);
	
	// Called when the health of the character goes below 0
	UFUNCTION()
	void CharDeath();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void MoveCharacter(FVector moveDir);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void UpdateLookingDirection(float rotation);

	// Attack functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void StartAttackCharge();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void InterruptAttackCharge();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void Attack(FVector attackDir);

	// Get and set functions
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	bool GetIsChargingAttack();
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	void SetIsChargingAttack(bool value);

	UFUNCTION(BlueprintCallable, Category = "GetSet")
	float GetHealthCurrent();
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	void SetHealthCurrent(float value);
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	float GetHealthMax();
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	void SetHealthMax(float value);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
