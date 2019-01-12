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

	UPROPERTY(replicated)
	float useless;

	//Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* cameraBase;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* cameraHolder;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* playerCamera;

	UPROPERTY()
	ABaseCharacter* controlledChar;
	UPROPERTY()
	FVector movementOnUpdateForward = FVector(0,0,0);
	UPROPERTY()
	FVector movementOnUpdateSideways = FVector(0,0,0);
	UPROPERTY()
	ABaseSlimeChar* slimeCharacter;
	UPROPERTY()
	bool IsControllingEnemy = false;

	// Camera settings
	UPROPERTY(EditAnywhere, Category = "Camera|Location")
	float cameraHeightOffset = 1200;
	UPROPERTY(EditAnywhere, Category = "Camera|Location")
	float cameraHorizontalOffset = -600;
	UPROPERTY(EditAnywhere, Category = "Camera|Location")
	float groundOffset = 0;
	UPROPERTY(EditAnywhere, Category = "Camera|Rotation")
	float cameraAngle = -60;
	UPROPERTY(EditAnywhere, Category = "Camera|Rotation")
	float cameraRotation = 0;

	UPROPERTY(EditAnywhere, Category = "Camera|Zoom")
	float cameraZoomSpeed = 100;
	UPROPERTY(EditAnywhere, Category = "Camera|Zoom")
	float cameraZoomMax = 0.8;
	UPROPERTY()
	float cameraCurrentZoomLength = 0;

	// Movements functions
	UFUNCTION()
	void MoveForwardCommand(float value = 0);
	UFUNCTION()
	void MoveRightCommand(float value = 0);

	// Camera control functions
	UFUNCTION()
	void CameraZoom(float value = 0);

	// Attack functions
	// Generic attack functions that acts like commands for the controlled pawn
	UFUNCTION()
	void StartAttackCharge(EAttackSlots attackSlotUsed);
	UFUNCTION()
	void CancelAttackCharge(EAttackSlots attackSlotUsed);
	UFUNCTION()
	void AttackCommand(EAttackSlots attackSlotUsed);

	// Used for inputs. These indicate when the button is pressed
	UFUNCTION()
	void AttackCommandCharge_BasicAttack();
	UFUNCTION()
	void AttackCommandCharge_SecondaryAttack();
	UFUNCTION()
	void AttackCommandCharge_Slot1();
	UFUNCTION()
	void AttackCommandCharge_Slot2();
	UFUNCTION()
	void AttackCommandCharge_Slot3();

	//Used for inputs. These indicate when the button is released
	UFUNCTION()
	void AttackCommandExe_BasicAttack();
	UFUNCTION()
	void AttackCommandExe_SecondaryAttack();
	UFUNCTION()
	void AttackCommandExe_Slot1();
	UFUNCTION()
	void AttackCommandExe_Slot2();
	UFUNCTION()
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
