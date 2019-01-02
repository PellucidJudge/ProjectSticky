// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "ProjectSticky/Interfaces/HealthManipulation.h"
#include "Abilities/BaseAbility.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EAttackSlots : uint8
{
	AS_BasicAttack		UMETA(DisplayName = "BasicAttack"),
	AS_SecondaryAttack	UMETA(DisplayName = "SecondaryAttack"),
	AS_Slot1			UMETA(DisplayName = "AttackSlot1"),
	AS_Slot2			UMETA(DisplayName = "AttackSlot2"),
	AS_Slot3			UMETA(DisplayName = "AttackSlot3")
};

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
	UPROPERTY(replicated, VisibleAnywhere, Category = "Combat")
	bool IsChargingAnAttack = false;

	UPROPERTY(replicated, EditAnywhere, Category = "CharacterStats")
	float movementSpeed = 600;
	UPROPERTY(replicated, EditAnywhere, Category = "CharacterStats")
	float movementSpeedMod = 1;
	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float chargingMovePenalty = 0.5;
	UPROPERTY(EditAnywhere, Category = "CharacterStats")
	float knockBackResistance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class ABaseAbility> BasicAbilityClass;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<class ABaseAbility> SecondaryAbilityClass;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<class ABaseAbility> Slot1AbilityClass;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<class ABaseAbility> Slot2AbilityClass;
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<class ABaseAbility> Slot3AbilityClass;

	UPROPERTY(replicated, VisibleAnywhere, Category = "Abilities")
	ABaseAbility* basicAbility;
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	ABaseAbility* secondaryAbility;
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	ABaseAbility* slot1Ability;
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	ABaseAbility* slot2Ability;
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	ABaseAbility* slot3Ability;

public:	

	UPROPERTY()
	int32 numberOfUsedAttackSlots;
	
	// Health manipulation interface functions
	UFUNCTION(BLueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void DamageObject(float damage, AActor* attacker, float knockBackDistance, FVector knockBackDir);
	
	// Called when the health of the character goes below 0
	UFUNCTION()
	void CharDeath();
	UFUNCTION(/*Server, Reliable, WithValidation,*/ Category = "Combat")
	void MoveCharacter(FVector moveDir);
	UFUNCTION(/*Server, Reliable, WithValidation,*/ Category = "Combat")
	void UpdateLookingDirection(float rotation);

	// Attack functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void StartAttackCharge(EAttackSlots attackSlotUsed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void InterruptAttackCharge();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void Attack(FVector attackDir, EAttackSlots attackSlotUsed);

	// Get and set functions
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	bool GetIsChargingAttack();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "GetSet")
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
