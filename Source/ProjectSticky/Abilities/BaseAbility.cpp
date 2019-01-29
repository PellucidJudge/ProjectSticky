// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAbility.h"
#include "Engine.h"


ABaseAbility::ABaseAbility() : Super()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	PrimaryActorTick.bCanEverTick = true;

}

void ABaseAbility::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool ABaseAbility::DamageActor(AActor * actor)
{
	return false;
}

// Called on clients to tell the server
// This is called from the controlling character when an ability is charged
void ABaseAbility::ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation) { }
// Validate that the call is correct
bool ABaseAbility::ChargeAbility_Validate(AActor * user, FVector direction, FVector mouseLocation) { return true; }
// Called on server to forward information and actually execute the ability
void ABaseAbility::ServerChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation) { }

// Called on clients to tell the server
// Called from the controlling character when the attack is supposed to be executed
void ABaseAbility::ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation) { }
// Validate that the call is correct
bool ABaseAbility::ExecuteAbility_Validate(AActor * user, FVector direction, FVector mouseLocation) { return true; }
// Called on server to forward information and actually execute the ability
void ABaseAbility::ServerExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation) { }


// Particle effect functions
// Useful for many if not nearly all of the child scripts

// Spawns the specified particle system and sets it to destroy when done, the simple and straightforward way.
void ABaseAbility::SpawnParticleEffect(UParticleSystem * particleSystem, FTransform worldTransform)
{
	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		if (particleSystem != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(world, particleSystem, worldTransform, true);
		}
	}
}

// Spawns a particle system that is meant to remain and be destroyed or deactivated later by script
UParticleSystemComponent* ABaseAbility::SpawnLastingParticleEffect(UParticleSystem * particleSystem, FTransform worldTransform)
{
	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		if (particleSystem != nullptr)
		{
			UParticleSystemComponent* particleSystemComp;
			particleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(world, particleSystem, worldTransform, false);

			return particleSystemComp;
		}
	}

	return nullptr;
}
