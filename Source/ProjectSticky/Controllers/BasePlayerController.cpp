// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"

ABasePlayerController::ABasePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bAutoManageActiveCameraTarget = false;

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Component setup
	root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;
	cameraHolder = CreateDefaultSubobject<USceneComponent>("CameraHolder");
	cameraHolder->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetIncludingScale);
	playerCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	playerCamera->AttachToComponent(cameraHolder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	playerCamera->Activate();

	// Set camera position and rotation
	FVector cameraLocOffset = FVector(cameraHorizontalOffset, 0, cameraHeightOffset);
	cameraHolder->SetRelativeLocation(cameraLocOffset);
	cameraHolder->SetWorldRotation(FRotator(cameraAngle, 0, 0));
}

void ABasePlayerController::BeginPlay()
{
	UpdateCharRef();
	if (playerCamera)
	{
		playerCamera->SetActive(true);
	}
}

void ABasePlayerController::Tick(float DeltaSeconds)
{
	if (controlledChar)
	{
		// Send movement input to possessed character
		controlledChar->MoveCharacter(movementOnUpdate);

		// Character rotation
		FHitResult Hit(ForceInit);
		FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Trace")), true, controlledChar);

		MouseLineTrace(&Hit, &Params);

		FVector lookDir = controlledChar->GetActorLocation() - Hit.Location;
		lookDir.GetSafeNormal(1);
		FRotator lookRot = lookDir.Rotation();
		if (lookDir != FVector(0, 0, 0))
		{
			controlledChar->UpdateLookingDirection(lookRot.Yaw);
		}

		// Move the camera to follow the possessed character
		root->SetWorldLocation(controlledChar->GetActorLocation());
	}

	
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRight);
}

void ABasePlayerController::CalcCamera(float DeltaTime, FMinimalViewInfo & OutResult)
{
	if (playerCamera != nullptr)
	{
		playerCamera->GetCameraView(DeltaTime, OutResult);
		return;
	}
	GetActorEyesViewPoint(OutResult.Location, OutResult.Rotation);
}

void ABasePlayerController::MoveForward(float value)
{
	movementOnUpdate.Y = value;
}

void ABasePlayerController::MoveRight(float value)
{
	movementOnUpdate.X = value;
}

void ABasePlayerController::UpdateCharRef()
{
	controlledChar = (ABaseCharacter*)GetPawn();
}

bool ABasePlayerController::MouseLineTrace(FHitResult* Hit, FCollisionQueryParams* Params)
{
	FVector mouseLocation;
	FVector mouseDirection;

	if (DeprojectMousePositionToWorld(mouseLocation, mouseDirection))
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FVector lineTraceStart = mouseLocation;
			FVector lineTraceEnd = mouseLocation + (mouseDirection * cameraHeightOffset * 2);

			Params->bTraceComplex = true;

			// Line trace
			bool DidTrace = world->LineTraceSingleByObjectType
				(
					*Hit, lineTraceStart, lineTraceEnd, ECC_WorldStatic, *Params
				);

			return DidTrace;
		}
	}

	return false;
}
