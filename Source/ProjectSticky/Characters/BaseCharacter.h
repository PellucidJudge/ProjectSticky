// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECTSTICKY_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Character stats
	UPROPERTY(EditAnywhere, Category = "CharacterStatus")
	float healthCurrent;
	UPROPERTY(EditAnywhere, Category = "CharacterStatus")
	float healthMax;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float damage;

public:	

	UFUNCTION()
	void CharDeath();
	UFUNCTION()
	void MoveCharacter(FVector moveDir);
	UFUNCTION()
	void UpdateLookingDirection(float rotation);

	// Attack functions
	UFUNCTION()
	void StartAttackCharge();
	UFUNCTION()
	void Attack(FVector attackDir);

	// Get and set functions
	UFUNCTION()
	float GetHealthCurrent();
	UFUNCTION()
	void SetHealthCurrent(float value);
	UFUNCTION()
	float GetHealthMax();
	UFUNCTION()
	void SetHealthMax(float value);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
