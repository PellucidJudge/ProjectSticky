// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "ProjectSticky/Characters/BaseCharacter.h"
#include "ProjectSticky/Characters/Slimes/BaseSlimeChar.h"
#include "ProjectSticky/Characters/Enemies/BaseEmenyChar.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTSTICKY_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ABasePlayerController();

protected:

	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* cameraHolder;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* playerCamera;

	UPROPERTY()
	ABaseCharacter* controlledChar;
	UPROPERTY()
	FVector movementOnUpdate = FVector(0,0,0);
	UPROPERTY()
	ABaseSlimeChar* slimeCharacter;
	UPROPERTY()
	bool IsControllingEnemy = false;

	// Camera settings
	UPROPERTY(EditAnywhere)
	float cameraHeightOffset = 800;
	UPROPERTY(EditAnywhere)
	float cameraHorizontalOffset = -400;
	UPROPERTY(EditAnywhere)
	float cameraAngle = -60;
	UPROPERTY(EditAnywhere)
	float cameraRotation = 0;
	UPROPERTY(EditAnywhere)
	float groundOffset = 0;

	// Movements functions
	UFUNCTION()
	void MoveForwardCommand(float value);
	UFUNCTION()
	void MoveRightCommand(float value);

	// Attack functions
	// Generic attack functions that acts like commands for the controlled pawn
	UFUNCTION()
	void StartAttackCharge(EAttackSlots attackSlotUsed);
	UFUNCTION()
	void CancelAttackCharge(EAttackSlots attackSlotUsed);
	UFUNCTION()
	void AttackCommand(EAttackSlots attackSlotUsed);

	// Used for inputs. These indicate when the button is pressed
	void AttackCommandCharge_BasicAttack();
	void AttackCommandCharge_SecondaryAttack();
	void AttackCommandCharge_Slot1();
	void AttackCommandCharge_Slot2();
	void AttackCommandCharge_Slot3();

	//Used for inputs. These indicate when the button is released
	void AttackCommandExe_BasicAttack();
	void AttackCommandExe_SecondaryAttack();
	void AttackCommandExe_Slot1();
	void AttackCommandExe_Slot2();
	void AttackCommandExe_Slot3();

	UFUNCTION()
	ABaseEmenyChar* TryPossessEnemy();
	UFUNCTION()
	bool TakePossessionOfEnemyChar(ABaseEmenyChar* enemyToPossess);
	UFUNCTION()
	void ReturnToSlimeForm();

	UFUNCTION()
	void UpdateCharRef();
	UFUNCTION()
	bool TraceSphere(AActor* actorToIngore, const FVector& startPoint, const FVector& endPoint, 
		const float radius, FHitResult& HitOut, ECollisionChannel TraceChannel = ECC_Pawn);

	bool MouseLineTrace(FHitResult* Hit, FCollisionQueryParams* Params);
	
public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	
};
