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

void ABaseCharacter::DamageObject_Implementation(float damage, AActor * attacker)
{
}

// Called when health reaches 0
void ABaseCharacter::CharDeath()
{
	this->Destroy();
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

void ABaseCharacter::StartAttackCharge_Implementation()
{
	SetIsChargingAttack(true);
}
void ABaseCharacter::InterruptAttackCharge_Implementation()
{
}
void ABaseCharacter::Attack_Implementation(FVector attackDir)
{
	//UE_LOG(LogTemp, Warning, TEXT("offset %s"), *attackDir.ToString());
	if (GetIsChargingAttack())
	{
		FVector locationOffset = attackDir * 200;
		this->SetActorLocation(this->GetActorLocation() + locationOffset);
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
