// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "UnrealNetwork.h"
#include "Engine.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	bReplicates = true;
	bReplicateMovement = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, movementSpeed);
	DOREPLIFETIME(ABaseCharacter, movementSpeedMod);
	DOREPLIFETIME(ABaseCharacter, IsChargingAnAttack);
}


// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Create ability instances
	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("beginplay"));
		if (BasicAbilityClass != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("create ability"));
			basicAbility = world->SpawnActor<ABaseAbility>(BasicAbilityClass);
			basicAbility->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			basicAbility->SetOwner(this);
		}
		if (SecondaryAbilityClass != nullptr)
		{
			secondaryAbility = world->SpawnActor<ABaseAbility>(SecondaryAbilityClass);
			secondaryAbility->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			secondaryAbility->SetOwner(this);
		}
		if (Slot1AbilityClass != nullptr)
		{
			slot1Ability = world->SpawnActor<ABaseAbility>(Slot1AbilityClass);
			slot1Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			slot1Ability->SetOwner(this);
		}
		if (Slot2AbilityClass != nullptr)
		{
			slot2Ability = world->SpawnActor<ABaseAbility>(Slot2AbilityClass);
			slot2Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			slot2Ability->SetOwner(this);
		}
		if (Slot3AbilityClass != nullptr)
		{
			slot3Ability = world->SpawnActor<ABaseAbility>(Slot3AbilityClass);
			slot3Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			slot3Ability->SetOwner(this);
		}
		
	}

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
/*
void ABaseCharacter::DamageObject_Implementation(int32 damage, AActor * attacker, float knockBackDistance, FVector knockBackDir)
{
	//UE_LOG(LogTemp, Display, TEXT("%s attacked %d for %f damage"), *attacker->GetName(), *GetName(), damage);

	FDamageStruct Damage;
	Damage.DamageAmount = 2;

	//SetHealthCurrent(GetHealthCurrent() - Damage.DamageAmount);

	if (GetHealthCurrent() <= 0)
	{
		CharDeath();
	}
}
*/
// Called when health reaches 0
void ABaseCharacter::CharDeath()
{
	//this->Destroy();
}

void ABaseCharacter::MoveCharacter(FVector moveDir)
{
	UWorld* world = GetWorld();
	if (world)
	{
		if (moveDir.GetSafeNormal(1) != FVector(0, 0, 0))
		{
			moveDir.Normalize(1);
			AddMovementInput(moveDir, movementSpeedMod);
		}
	}
}

void ABaseCharacter::UpdateLookingDirection(float rotation)
{
	if (rotateToFaceMoveDirection == false)
	{
		FRotator lookDirection = FRotator(0, 0, 0);
		lookDirection.Roll = this->GetActorRotation().Roll;
		lookDirection.Pitch = this->GetActorRotation().Pitch;
		lookDirection.Yaw = rotation;

		this->SetActorRotation(lookDirection);
	}
	
}

void ABaseCharacter::StartAttackCharge_Implementation(FVector attackDir, EAttackSlots attackSlotUsed, FVector mouseLocation)
{
	switch (attackSlotUsed)
	{
	case EAttackSlots::AS_BasicAttack:
		if (basicAbility != nullptr)
		{
			SetIsChargingAttack(true);
			basicAbility->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_SecondaryAttack:
		if (secondaryAbility != nullptr)
		{
			SetIsChargingAttack(true);
			secondaryAbility->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_Slot1:
		if (slot1Ability != nullptr)
		{
			SetIsChargingAttack(true);
			slot1Ability->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_Slot2:
		if (slot2Ability != nullptr)
		{
			SetIsChargingAttack(true);
			slot2Ability->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_Slot3:
		if (slot3Ability != nullptr)
		{
			SetIsChargingAttack(true);
			slot3Ability->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;
	}
}
void ABaseCharacter::InterruptAttackCharge_Implementation()
{
}
void ABaseCharacter::Attack_Implementation(FVector attackDir, EAttackSlots attackSlotUsed, FVector mouseLocation)
{
	

	switch (attackSlotUsed)
	{
	case EAttackSlots::AS_BasicAttack:
		if (GetIsChargingAttack())
		{
			if (basicAbility != nullptr)
			{
				basicAbility->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_SecondaryAttack:
		if (GetIsChargingAttack())
		{
			if (secondaryAbility != nullptr)
			{
				secondaryAbility->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_Slot1:
		if (GetIsChargingAttack())
		{
			if (slot1Ability != nullptr)
			{
				slot1Ability->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_Slot2:
		if (GetIsChargingAttack())
		{
			if (slot2Ability != nullptr)
			{
				slot2Ability->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_Slot3:
		if (GetIsChargingAttack())
		{
			if (slot3Ability != nullptr)
			{
				slot3Ability->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;
	}

	SetIsChargingAttack(false);
}


// Get and set functions
bool ABaseCharacter::GetIsChargingAttack() { return IsChargingAnAttack; }
void ABaseCharacter::SetIsChargingAttack_Implementation(bool value) 
{ 
	IsChargingAnAttack = value; 
	if (value == true)
	{
		movementSpeedMod = 0.5;
	}
	else
	{
		movementSpeedMod = 1;
	}
}
bool ABaseCharacter::SetIsChargingAttack_Validate(bool value)
{
	return true;
}

float ABaseCharacter::GetHealthCurrent(){ return healthCurrent; }
void ABaseCharacter::SetHealthCurrent(float value){ healthCurrent = value; }

float ABaseCharacter::GetHealthMax(){ return healthMax; }
void ABaseCharacter::SetHealthMax(float value) { healthMax = value; }
