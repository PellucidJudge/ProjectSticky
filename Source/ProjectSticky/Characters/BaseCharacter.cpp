// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

void ABaseCharacter::CharDeath()
{
	this->Destroy();
}

void ABaseCharacter::MoveCharacter(FVector moveDir)
{
	UWorld* world = GetWorld();
	if (world)
	{
		moveDir.GetSafeNormal(1);
		GetCharacterMovement()->MoveSmooth(GetCharacterMovement()->MaxWalkSpeed * moveDir, world->GetDeltaSeconds());
	}
}

void ABaseCharacter::UpdateLookingDirection(float rotation)
{
	FRotator lookDirection = FRotator(0,0,0);
	lookDirection.Roll = this->GetActorRotation().Roll;
	lookDirection.Pitch = this->GetActorRotation().Pitch;
	lookDirection.Yaw = rotation;

	this->SetActorRotation(lookDirection);
}

void ABaseCharacter::StartAttackCharge()
{
}

void ABaseCharacter::Attack(FVector attackDir)
{
}


// Get and set functions
float ABaseCharacter::GetHealthCurrent()
{
	return healthCurrent;
}

void ABaseCharacter::SetHealthCurrent(float value)
{
	healthCurrent = value;
}

float ABaseCharacter::GetHealthMax()
{
	return healthMax;
}

void ABaseCharacter::SetHealthMax(float value)
{
	healthMax = value;
}
