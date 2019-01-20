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

	// Character settings
	UPROPERTY(EditAnywhere, Category = "Settings|Movement")
	bool rotateToFaceMoveDirection = false;

	// Character stats
	UPROPERTY(EditAnywhere, Category = "Stats|Defense")
	float healthMax = 3;
	UPROPERTY(EditAnywhere, Category = "Stats|Defense")
	float healthStart = 3;
	UPROPERTY(EditAnywhere, Category = "Stats|Defense")
	float healthCurrent = 3;
	UPROPERTY(EditAnywhere, Category = "Stats|Defense")
	float armorStart = 0;
	UPROPERTY(EditAnywhere, Category = "Stats|Defense")
	float armorCurrent = 0;
	UPROPERTY(EditAnywhere, Category = "Stats|Defense")
	FDefenceWeaknessStruct defenceAndWeaknesess;
	UPROPERTY(EditAnywhere, Category = "Stats|Combat")
	float CharDamage;
	UPROPERTY(replicated, VisibleAnywhere, Category = "Stats|Combat")
	bool IsChargingAnAttack = false;

	UPROPERTY(replicated, EditAnywhere, Category = "Stats|Utility")
	float movementSpeed = 600;
	UPROPERTY(replicated, EditAnywhere, Category = "Stats|Utility")
	float movementSpeedMod = 1;
	UPROPERTY(EditAnywhere, Category = "Stats|Utility")
	float chargingMovePenalty = 0.5;
	UPROPERTY(EditAnywhere, Category = "Stats|Combat")
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

	UPROPERTY(VisibleAnywhere, Category = "Abilities")
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
	void DamageObject(TArray<FDamageStruct>& damage, AActor* attacker, float knockBackDistance, FVector knockBackDir);

	// Damage functions
	UFUNCTION()
	void StandardDamage(float damageAmount);

	// Called when the health of the character goes below 0
	UFUNCTION()
	void CharDeath();
	UFUNCTION()
	void MoveCharacter(FVector moveDir);
	UFUNCTION()
	void UpdateLookingDirection(float rotation);

	// Attack functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void StartAttackCharge(FVector attackDir, EAttackSlots attackSlotUsed, FVector mouseLocation);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void InterruptAttackCharge();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void Attack(FVector attackDir, EAttackSlots attackSlotUsed, FVector mouseLocation);

	// Get and set functions
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	bool GetIsChargingAttack();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "GetSet")
	void SetIsChargingAttack(bool value);

	// Get and set functions for health
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	float GetHealthCurrent();
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	void SetHealthCurrent(float value);
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	float GetHealthMax();
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	void SetHealthMax(float value);

	// Get and set for armor
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	void SetArmorCurrent(float value);
	UFUNCTION(BlueprintCallable, Category = "GetSet")
	float GetArmorCurrent();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
