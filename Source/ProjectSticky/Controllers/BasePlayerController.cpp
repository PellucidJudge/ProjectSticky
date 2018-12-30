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

		// Line trace to use with the different functions in this frame
		FHitResult Hit(ForceInit);
		FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Trace")), true, controlledChar);
		MouseLineTrace(&Hit, &Params);

		// Character rotation
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
	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForwardCommand);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRightCommand);
	InputComponent->BindAction("BasicAttack", IE_Pressed, this, &ABasePlayerController::AttackCommandCharge_BasicAttack);
	InputComponent->BindAction("BasicAttack", IE_Released, this, &ABasePlayerController::AttackCommandExe_BasicAttack);
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

//_____________________________________________
// Move Commands
void ABasePlayerController::MoveForwardCommand(float value)
{
	movementOnUpdate.Y = value;
}
void ABasePlayerController::MoveRightCommand(float value)
{
	movementOnUpdate.X = value;
}

//______________________________________________
// Attack commands
void ABasePlayerController::StartAttackCharge(EAttackSlots attackSlotUsed)
{
	if (controlledChar)
	{
		controlledChar->StartAttackCharge();
	}
}
void ABasePlayerController::CancelAttackCharge(EAttackSlots attackSlotUsed)
{
	if (controlledChar)
	{

	}
}
void ABasePlayerController::AttackCommand(EAttackSlots attackSlotUsed)
{
	if (controlledChar)
	{
		FHitResult Hit(ForceInit);
		FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Trace")), true, controlledChar);
		MouseLineTrace(&Hit, &Params);

		FVector attackDir = Hit.Location - controlledChar->GetActorLocation();
		
		if (attackDir.GetSafeNormal(1) != FVector(0, 0, 0))
		{
			attackDir.Normalize(1);
			controlledChar->Attack(attackDir);
		}
	}
}

// These attack command functions are unique per input and then provide the identifier to a generic function
// These start attack charging for the respective slot.
void ABasePlayerController::AttackCommandCharge_BasicAttack() { StartAttackCharge(EAttackSlots::AS_BasicAttack); }
void ABasePlayerController::AttackCommandCharge_SecondaryAttack() { StartAttackCharge(EAttackSlots::AS_SecondaryAttack); }
void ABasePlayerController::AttackCommandCharge_Slot1() { StartAttackCharge(EAttackSlots::AS_Slot1); }
void ABasePlayerController::AttackCommandCharge_Slot2() { StartAttackCharge(EAttackSlots::AS_Slot2); }
void ABasePlayerController::AttackCommandCharge_Slot3() { StartAttackCharge(EAttackSlots::AS_Slot3); }

// When the player releases the attack button or other criterias are met the attack goes through.
void ABasePlayerController::AttackCommandExe_BasicAttack() { AttackCommand(EAttackSlots::AS_BasicAttack); }
void ABasePlayerController::AttackCommandExe_SecondaryAttack() { AttackCommand(EAttackSlots::AS_SecondaryAttack); }
void ABasePlayerController::AttackCommandExe_Slot1() { AttackCommand(EAttackSlots::AS_Slot1); }
void ABasePlayerController::AttackCommandExe_Slot2() { AttackCommand(EAttackSlots::AS_Slot2); }
void ABasePlayerController::AttackCommandExe_Slot3() { AttackCommand(EAttackSlots::AS_Slot3); }

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
