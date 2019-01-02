// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"

ABasePlayerController::ABasePlayerController()
{
	bReplicates = true;

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

void ABasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABasePlayerController, useless);
	//DOREPLIFETIME(ABasePlayerController, movementOnUpdate);
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
	InputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ABasePlayerController::AttackCommandCharge_SecondaryAttack);
	InputComponent->BindAction("SecondaryAttack", IE_Released, this, &ABasePlayerController::AttackCommandExe_SecondaryAttack);
	InputComponent->BindAction("LeavePossessedBody", IE_Pressed, this, &ABasePlayerController::ReturnToSlimeForm);
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
void ABasePlayerController::MoveForwardCommand/*_Implementation*/(float value)
{
	movementOnUpdate.Y = value;
}
/*bool ABasePlayerController::MoveForwardCommand_Validate(float value)
{
	return true;
}
*/
void ABasePlayerController::MoveRightCommand/*_Implementation*/(float value)
{
	movementOnUpdate.X = value;
}
/*bool ABasePlayerController::MoveRightCommand_Validate(float value)
{
	return true;
}
*/
//______________________________________________
// Attack commands
void ABasePlayerController::StartAttackCharge(EAttackSlots attackSlotUsed)
{
	if (controlledChar)
	{
		controlledChar->StartAttackCharge(attackSlotUsed);
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
			controlledChar->Attack(attackDir, attackSlotUsed);
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
void ABasePlayerController::AttackCommandExe_SecondaryAttack() 
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
	AttackCommand(EAttackSlots::AS_SecondaryAttack); 
}
void ABasePlayerController::AttackCommandExe_Slot1() { AttackCommand(EAttackSlots::AS_Slot1); }
void ABasePlayerController::AttackCommandExe_Slot2() { AttackCommand(EAttackSlots::AS_Slot2); }
void ABasePlayerController::AttackCommandExe_Slot3() { AttackCommand(EAttackSlots::AS_Slot3); }

// Enemy possession functions
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
