// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"
#include "Engine.h"

ABasePlayerController::ABasePlayerController() : Super()
{

	bReplicates = false;

	PrimaryActorTick.bCanEverTick = true;
	bAutoManageActiveCameraTarget = false;

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Component setup
	root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;
	cameraBase = CreateDefaultSubobject<USceneComponent>("CameraBase");
	cameraBase->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetIncludingScale);
	cameraHolder = CreateDefaultSubobject<USceneComponent>("CameraHolder");
	cameraHolder->AttachToComponent(cameraBase, FAttachmentTransformRules::SnapToTargetIncludingScale);
	playerCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	playerCamera->AttachToComponent(cameraHolder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	playerCamera->Activate();

	// Set camera position and rotation
	FVector cameraLocOffset = FVector(cameraHorizontalOffset, 0, cameraHeightOffset);
	cameraHolder->SetRelativeLocation(cameraLocOffset);
	cameraHolder->SetWorldRotation(FRotator(cameraAngle, 0, 0));
	cameraBase->SetWorldRotation(FRotator(0, cameraRotation, 0));
}

void ABasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABasePlayerController, useless);
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UpdateCharRef();
	if (playerCamera)
	{
		playerCamera->SetActive(true);
	}
}

void ABasePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (controlledChar)
	{
		// Send movement input to possessed character
		FVector movementOnUpdate = movementOnUpdateSideways + movementOnUpdateForward;
		controlledChar->MoveCharacter(movementOnUpdate);

		//UE_LOG(LogTemp, Warning, TEXT("MoveRight %s"), *movementOnUpdateSideways.ToString());

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

// Here is whera all player input is set up
void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Movement
	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForwardCommand);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRightCommand);

	// Camera controls
	InputComponent->BindAxis("CameraZoom", this, &ABasePlayerController::CameraZoom);

	// Combat
	InputComponent->BindAction("BasicAttack", IE_Pressed, this, &ABasePlayerController::AttackCommandCharge_BasicAttack);
	InputComponent->BindAction("BasicAttack", IE_Released, this, &ABasePlayerController::AttackCommandExe_BasicAttack);
	InputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ABasePlayerController::AttackCommandCharge_SecondaryAttack);
	InputComponent->BindAction("SecondaryAttack", IE_Released, this, &ABasePlayerController::AttackCommandExe_SecondaryAttack);
	InputComponent->BindAction("LeavePossessedBody", IE_Pressed, this, &ABasePlayerController::ReturnToSlimeForm);
	InputComponent->BindAction("PossessBody", IE_Pressed, this, &ABasePlayerController::Command_PossessEnemy);
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
// Move commands
void ABasePlayerController::MoveForwardCommand(float value)
{
	FVector movementForwardAmount = FVector(0, value, 0);
	FRotator movementForwardRot = FRotator(0, 0, cameraRotation);
	FVector movementForward = movementForwardRot.RotateVector(movementForwardAmount);

	movementOnUpdateForward = movementForward;
}

void ABasePlayerController::MoveRightCommand(float value)
{
	
	FVector movementSidewaysAmount = FVector(value, 0, 0);
	FRotator movementSidewaysRot = FRotator(0, cameraRotation, 0);
	FVector movementSideways = movementSidewaysRot.RotateVector(movementSidewaysAmount);
	
	//UE_LOG(LogTemp, Warning, TEXT("MoveRight %s"), *movementSidewaysRot.ToString());

	movementOnUpdateSideways = movementSideways;
}

//______________________________________________
// Camera control commands
void ABasePlayerController::CameraZoom(float value)
{
	// DOES NOT WORK, CRASHES THE EDITOR AS LONG AS THE LOG COMMAND IS THERE
	// Crashes when you press a button connected to this function
	/*
	if (InputComponent != nullptr)
	{
		// Calculate values used when figuring out the camera distance
		float cameraMaxLengthFrombase = FMath::Sqrt(FMath::Square(cameraHeightOffset) + FMath::Square(cameraHorizontalOffset));
		float cameraMinLengthFrombase = cameraMaxLengthFrombase * (1 - cameraZoomMax);

		cameraCurrentZoomLength += FMath::Clamp(value * cameraZoomSpeed, cameraMaxLengthFrombase * cameraZoomMax, cameraMaxLengthFrombase);
		float zoomOffsetVertical = FMath::Sqrt(FMath::Square(cameraMaxLengthFrombase) - FMath::Square(cameraHorizontalOffset));
		float zoomOffsetHorizontal = FMath::Sqrt(FMath::Square(cameraMaxLengthFrombase) - FMath::Square(cameraHeightOffset));
		FVector newZoomOffset = FVector(0, -zoomOffsetHorizontal, zoomOffsetVertical);

		UE_LOG(LogTemp, Warning, TEXT("MoveRight %s"), value);

		// Add offsets together and set the new location of the camera
		FVector newCameraLocation = newZoomOffset;
		cameraHolder->SetRelativeLocation(newCameraLocation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent NULL"));
	}
	*/
}

//______________________________________________
// Attack commands
void ABasePlayerController::StartAttackCharge(EAttackSlots attackSlotUsed)
{
	if (controlledChar)
	{
		FHitResult Hit(ForceInit);
		FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Trace")), true, controlledChar);
		MouseLineTrace(&Hit, &Params);

		FVector attackDir = Hit.Location - controlledChar->GetActorLocation();

		controlledChar->StartAttackCharge(attackDir, attackSlotUsed, Hit.Location);
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
			controlledChar->Attack(attackDir, attackSlotUsed, Hit.Location);
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

// Enemy possession functions
// Possess command
void ABasePlayerController::Command_PossessEnemy()
{
	if (IsControllingEnemy == false)
	{
		ABaseEmenyChar* enemy = TryPossessEnemy();
		if (enemy != nullptr)
		{
			TakePossessionOfEnemyChar(enemy);
			IsControllingEnemy = true;
		}
	}
}

ABaseEmenyChar* ABasePlayerController::TryPossessEnemy()
{
	if (slimeCharacter != nullptr)
	{
		FHitResult Hit(ForceInit);
		FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("Trace")), true, controlledChar);
		MouseLineTrace(&Hit, &Params);

		FHitResult SphereHit(ForceInit);
		TraceSphere(slimeCharacter, Hit.Location, Hit.Location, 150, SphereHit, ECC_Pawn);
		if (SphereHit.bBlockingHit)
		{
			if (SphereHit.GetActor() != nullptr)
			{
				if (ABaseEmenyChar* enemyToControl = Cast<ABaseEmenyChar>(SphereHit.GetActor()))
				{
					return enemyToControl;
				}
			}
			
		}
	}
	return nullptr;
}

bool ABasePlayerController::TakePossessionOfEnemyChar(ABaseEmenyChar * enemyToPossess)
{
	if (enemyToPossess != nullptr)
	{
		Possess(enemyToPossess);
		UpdateCharRef();

		if (slimeCharacter != nullptr)
		{
			slimeCharacter->SetSlimeActive(false);
		}
	}

	return false;
}

void ABasePlayerController::ReturnToSlimeForm()
{
	if (slimeCharacter != nullptr)
	{
		slimeCharacter->SetSlimeActive(true);
		slimeCharacter->SetActorLocation(controlledChar->GetActorLocation());

		Possess(slimeCharacter);
		UpdateCharRef();

		IsControllingEnemy = false;
	}
}

void ABasePlayerController::UpdateCharRef()
{
	// If no slime is defined to go back to then check if the current character is a slime.
	if (slimeCharacter == nullptr)
	{
		if (ABaseSlimeChar* temp = Cast<ABaseSlimeChar>(GetPawn()))
		{
			slimeCharacter = temp;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning: No slime found."));
		}
	}

	// Update the currently controlled char.
	controlledChar = (ABaseCharacter*)GetPawn();
}

bool ABasePlayerController::TraceSphere(AActor * actorToIngore, const FVector & startPoint, const FVector & endPoint, 
	const float radius, FHitResult & HitOut, ECollisionChannel TraceChannel)
{
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, actorToIngore);
	TraceParams.bReturnPhysicalMaterial = false;

	HitOut = FHitResult(ForceInit);

	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		FColor color = FColor(1, 0, 0, 1);
		DrawDebugSphere(world, startPoint, radius, 20, color);

		world->SweepSingleByObjectType(
			HitOut,
			startPoint,
			endPoint,
			FQuat(),
			TraceChannel,
			FCollisionShape::MakeSphere(radius),
			TraceParams
		);
	}

	return false;
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
