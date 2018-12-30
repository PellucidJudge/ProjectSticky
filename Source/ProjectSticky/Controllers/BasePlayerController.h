// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "ProjectSticky/Characters/BaseCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */

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

	UFUNCTION()
	void MoveForwardCommand(float value);
	UFUNCTION()
	void MoveRightCommand(float value);

	// Attack functions
	UFUNCTION()
	void StartAttackCharge(EAttackSlots attackSlotUsed);
	UFUNCTION()
	void CancelAttackCharge(EAttackSlots attackSlotUsed);
	UFUNCTION()
	void AttackCommand(EAttackSlots attackSlotUsed);

	void AttackCommandCharge_BasicAttack();
	void AttackCommandCharge_SecondaryAttack();
	void AttackCommandCharge_Slot1();
	void AttackCommandCharge_Slot2();
	void AttackCommandCharge_Slot3();

	void AttackCommandExe_BasicAttack();
	void AttackCommandExe_SecondaryAttack();
	void AttackCommandExe_Slot1();
	void AttackCommandExe_Slot2();
	void AttackCommandExe_Slot3();

	UFUNCTION()
	void UpdateCharRef();

	bool MouseLineTrace(FHitResult* Hit, FCollisionQueryParams* Params);
	
public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	
};
