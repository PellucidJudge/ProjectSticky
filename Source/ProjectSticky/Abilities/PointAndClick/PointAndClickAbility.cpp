// Fill out your copyright notice in the Description page of Project Settings.

#include "PointAndClickAbility.h"
#include "ProjectSticky/Interfaces/HealthManipulation.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Engine.h"

APointAndClickAbility::APointAndClickAbility() : Super()
{

}

void APointAndClickAbility::BeginPlay()
{
	Super::BeginPlay();
}

void APointAndClickAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (isCharging)
	{
		float newScale = FMath::Lerp(1.0f, psEndSize, psCurrentSizeProgression);
		//chargeParticleSystem->SetWorldScale3D(FVector(newScale, newScale, newScale));

		psCurrentSizeProgression += DeltaSeconds;
		currentChargeTime += DeltaSeconds;
	}
}

// Server function
void APointAndClickAbility::Server_ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	Super::Server_ChargeAbility_Implementation(user, direction, mouseLocation);

	isCharging = true;
	currentChargeTime = 0;
	direction = FVector(direction.X, direction.Y, 0);
	startPos = mouseLocation;

	Multi_ChargeAbility(user, direction, mouseLocation);
}

// Function called on everything
void APointAndClickAbility::Multi_ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	Super::Multi_ChargeAbility_Implementation(user, direction, mouseLocation);

	psCurrentSizeProgression = 0;
}

// Server function
void APointAndClickAbility::Server_ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	Super::Server_ExecuteAbility_Implementation(user, direction, mouseLocation);

	if (user != nullptr)
	{
		FVector startPoint = user->GetActorLocation();

		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			TArray<AActor*> actorsToIgnoreDamage;
			Damage_SphereTrace(startPos, baseRange, actorsToIgnoreDamage);
		}
	}

	Multi_ExecuteAbility(user, direction, startPos);
}
// Executed on everything
void APointAndClickAbility::Multi_ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	Super::Multi_ExecuteAbility_Implementation(user, direction, mouseLocation);
}

