// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	healthCurrent = healthStart;

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
		}
		if (SecondaryAbilityClass != nullptr)
		{
			secondaryAbility = world->SpawnActor<ABaseAbility>(SecondaryAbilityClass);
			secondaryAbility->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		if (Slot1AbilityClass != nullptr)
		{
			slot1Ability = world->SpawnActor<ABaseAbility>(Slot1AbilityClass);
			slot1Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		if (Slot2AbilityClass != nullptr)
		{
			slot2Ability = world->SpawnActor<ABaseAbility>(Slot2AbilityClass);
			slot2Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		if (Slot3AbilityClass != nullptr)
		{
			slot3Ability = world->SpawnActor<ABaseAbility>(Slot3AbilityClass);
			slot3Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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

void ABaseCharacter::DamageObject_Implementation(float damage, AActor * attacker, float knockBackDistance, FVector knockBackDir)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));

	SetHealthCurrent(GetHealthCurrent() - damage);

	if (GetHealthCurrent() <= 0)
	{
		CharDeath();
	}
}

// Called when health reaches 0
void ABaseCharacter::CharDeath()
{
	//this->Destroy();
}

void ABaseCharacter::MoveCharacter_Implementation(FVector moveDir)
{
	UWorld* world = GetWorld();
	if (world)
	{
		if (moveDir.GetSafeNormal(1) != FVector(0, 0, 0))
		{
			moveDir.Normalize(1);
			GetCharacterMovement()->MoveSmooth(GetCharacterMovement()->MaxWalkSpeed * moveDir, world->GetDeltaSeconds());
		}
	}
}

void ABaseCharacter::UpdateLookingDirection_Implementation(float rotation)
{
	FRotator lookDirection = FRotator(0,0,0);
	lookDirection.Roll = this->GetActorRotation().Roll;
	lookDirection.Pitch = this->GetActorRotation().Pitch;
	lookDirection.Yaw = rotation;

	this->SetActorRotation(lookDirection);
}

void ABaseCharacter::StartAttackCharge_Implementation(EAttackSlots attackSlotUsed)
{
	switch (attackSlotUsed)
	{
	case EAttackSlots::AS_BasicAttack:
		if (basicAbility != nullptr)
		{
			SetIsChargingAttack(true);
		}
		break;

	case EAttackSlots::AS_SecondaryAttack:
		if (secondaryAbility != nullptr)
		{
			SetIsChargingAttack(true);
		}
		break;

	case EAttackSlots::AS_Slot1:
		if (slot1Ability != nullptr)
		{
			SetIsChargingAttack(true);
		}
		break;

	case EAttackSlots::AS_Slot2:
		if (slot2Ability != nullptr)
		{
			SetIsChargingAttack(true);
		}
		break;

	case EAttackSlots::AS_Slot3:
		if (slot3Ability != nullptr)
		{
			SetIsChargingAttack(true);
		}
		break;
	}
}
void ABaseCharacter::InterruptAttackCharge_Implementation()
{
}
void ABaseCharacter::Attack_Implementation(FVector attackDir, EAttackSlots attackSlotUsed)
{
	switch (attackSlotUsed)
	{
	case EAttackSlots::AS_BasicAttack:
		if (GetIsChargingAttack())
		{
			if (basicAbility != nullptr)
			{
				basicAbility->ExecuteAbility(this, attackDir, 200);
			}
		}
		break;

	case EAttackSlots::AS_SecondaryAttack:
		if (GetIsChargingAttack())
		{
			if (secondaryAbility != nullptr)
			{
				secondaryAbility->ExecuteAbility(this, attackDir, 200);
			}
		}
		break;

	case EAttackSlots::AS_Slot1:
		if (GetIsChargingAttack())
		{
			if (slot1Ability != nullptr)
			{
				slot1Ability->ExecuteAbility(this, attackDir, 200);
			}
		}
		break;

	case EAttackSlots::AS_Slot2:
		if (GetIsChargingAttack())
		{
			if (slot2Ability != nullptr)
			{
				slot2Ability->ExecuteAbility(this, attackDir, 200);
			}
		}
		break;

	case EAttackSlots::AS_Slot3:
		if (GetIsChargingAttack())
		{
			if (slot3Ability != nullptr)
			{
				slot3Ability->ExecuteAbility(this, attackDir, 200);
			}
		}
		break;
	}

	SetIsChargingAttack(false);
}


// Get and set functions
bool ABaseCharacter::GetIsChargingAttack() { return IsChargingAnAttack; }
void ABaseCharacter::SetIsChargingAttack(bool value) 
{ 
	IsChargingAnAttack = value; 
	if (value == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed - (MovementSpeed * chargingMovePenalty);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	}
}

float ABaseCharacter::GetHealthCurrent(){ return healthCurrent; }
void ABaseCharacter::SetHealthCurrent(float value){ healthCurrent = value; }

float ABaseCharacter::GetHealthMax(){ return healthMax; }
void ABaseCharacter::SetHealthMax(float value) { healthMax = value; }
