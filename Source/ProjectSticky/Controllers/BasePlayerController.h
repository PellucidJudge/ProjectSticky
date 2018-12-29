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
	FVector movementOnUpdate;

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
	void MoveForward(float value);
	UFUNCTION()
	void MoveRight(float value);
	UFUNCTION()
	void UpdateCharRef();

	bool MouseLineTrace(FHitResult* Hit, FCollisionQueryParams* Params);
	
public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	
};
