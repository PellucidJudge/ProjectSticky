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

	if (isCharging)
	{
		if (currentChargeTime >= fullChargeTime)
		{
			if (chargeParticleSystem != nullptr)
			{
				chargeParticleSystem->SetActive(false);
				chargeParticleSystem->SetVisibility(false);
			}
			if (chargeParticleSystemLoop != nullptr &&
				(!chargeParticleSystemLoop->IsActive() && !chargeParticleSystemLoop->IsVisible()))
			{
				chargeParticleSystemLoop->SetActive(true);
				chargeParticleSystemLoop->SetVisibility(true);
			}
		}
	}
}

bool ABaseAbility::DamageActor(AActor * actor)
{
	return false;
}

/*___________________________
	Ability functions
*/

void ABaseAbility::ChargeAbility(AActor * user, FVector direction, FVector mouseLocation)
{
	Server_ChargeAbility(user, direction, mouseLocation);
}
// Called on clients to tell the server
// This is called from the controlling character when an ability is charged
void ABaseAbility::Server_ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{

}
// Validate that the call is correct
bool ABaseAbility::Server_ChargeAbility_Validate(AActor * user, FVector direction, FVector mouseLocation) { return true; }
// Called on server to forward information and actually execute the ability
void ABaseAbility::Multi_ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	UWorld* world = GetWorld();
	if (world != nullptr && user != nullptr)
	{
		if (PS_AbilityCharge != nullptr)
		{
			FTransform spawnTransform;
			spawnTransform.SetLocation(mouseLocation);
			spawnTransform.SetRotation(direction.ToOrientationQuat());
			spawnTransform.SetScale3D(FVector(1, 1, 1));
			chargeParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(world, PS_AbilityCharge, spawnTransform, true);
		}
		if (PS_AbilityChargeLoop != nullptr && chargeParticleSystemLoop == nullptr)
		{
			FTransform spawnTransform;
			spawnTransform.SetLocation(mouseLocation);
			spawnTransform.SetRotation(direction.ToOrientationQuat());
			spawnTransform.SetScale3D(FVector(1, 1, 1));
			chargeParticleSystemLoop = UGameplayStatics::SpawnEmitterAtLocation(world, PS_AbilityChargeLoop, spawnTransform, false);

			chargeParticleSystemLoop->SetActive(false);
			chargeParticleSystemLoop->SetVisibility(false);
		}
		else if (PS_AbilityChargeLoop != nullptr)
		{
			chargeParticleSystemLoop->SetWorldLocation(mouseLocation);
			chargeParticleSystemLoop->SetWorldRotation(direction.ToOrientationQuat());
			chargeParticleSystemLoop->SetWorldScale3D(FVector(1, 1, 1));
		}
	}
}

void ABaseAbility::ExecuteAbility(AActor * user, FVector direction, FVector mouseLocation)
{
	Server_ExecuteAbility(user, direction, mouseLocation);
}
// Called on clients to tell the server
// Called from the controlling character when the attack is supposed to be executed
void ABaseAbility::Server_ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation) { }
// Validate that the call is correct
bool ABaseAbility::Server_ExecuteAbility_Validate(AActor * user, FVector direction, FVector mouseLocation) { return true; }
// Called on server to forward information and actually execute the ability
void ABaseAbility::Multi_ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	isCharging = false;
	direction = FVector(direction.X, direction.Y, 0);

	// Disable charge particle effects
	if (chargeParticleSystem != nullptr)
	{
		chargeParticleSystem->SetActive(false);
		chargeParticleSystem->SetVisibility(false);
	}

	if (chargeParticleSystemLoop != nullptr)
	{
		chargeParticleSystemLoop->SetActive(false);
		chargeParticleSystemLoop->SetVisibility(false);
	}

	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		// Play particle effect
		if (PS_AbilityExecutionPE != nullptr)
		{
			FTransform spawnTransform;
			spawnTransform.SetLocation(mouseLocation);
			spawnTransform.SetRotation(direction.ToOrientationQuat());
			spawnTransform.SetScale3D(FVector(1, 1, 1));

			UGameplayStatics::SpawnEmitterAtLocation(world, PS_AbilityExecutionPE, spawnTransform, true);
		}
	}
}

/*___________________________
	Hit Detection functions
*/
bool ABaseAbility::Damage_BoxTrace(FVector location, FVector boxDimensions, TArray<AActor*> actorsToIgnore)
{
	return false;
}

bool ABaseAbility::Damage_SphereTrace(FVector location, float radius, TArray<AActor*> actorsToIgnore)
{
	if (Role == ROLE_Authority)
	{
		TArray<AActor*> actorsToNotDamage;
		actorsToNotDamage = actorsToIgnore;

		if (currentUser != nullptr)
		{
			UWorld* world = GetWorld();
			if (world != nullptr)
			{
				if (IsDebugActive)
				{
					// Debug hit detection
					DrawDebugSphere(world, location, radius, 16, FColor::Green, false, 2.0f);
					DrawDebugLine(world, currentUser->GetActorLocation() + FVector(0, 0, 200), location, FColor::Red, false, 2.0f);
				}

				// Hit detection prep
				TArray<FHitResult> outHits;
				FCollisionObjectQueryParams ObjectParams;
				ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
				FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("BoxTrace")), true, currentUser);
				FCollisionShape Sphere = FCollisionShape::MakeSphere(radius);

				// Spheretrace
				bool anyHit = world->SweepMultiByObjectType(outHits, location, location,
					FQuat(0, 0, 0, 0), ObjectParams, Sphere, Params);

				// Check trace and damage appropriate actors
				if (anyHit)
				{
					// Cycle through hits and damage all actors in the sphere once
					for (auto& Hit : outHits)
					{
						if (Hit.Actor != nullptr && !actorsToNotDamage.Contains(Hit.Actor))
						{
							AActor* hitActor = Cast<AActor>(Hit.Actor);
							IHealthManipulation* HMInterface = Cast<IHealthManipulation>(Hit.Actor);
							// Add hit actor to ignore list so that it is not damaged further by the same attack.
							actorsToNotDamage.Add(hitActor);

							if (HMInterface != nullptr && hitActor != nullptr)
							{
								FVector damageDirection = hitActor->GetActorLocation() - location;
								if (damageDirection.GetSafeNormal(1) != FVector(0, 0, 0))
								{
									damageDirection.Normalize(1);
								}
								else damageDirection = FVector(1, 0, 0);

								HMInterface->Execute_DamageObject(hitActor, abilityDamage, currentUser, 100, damageDirection);

								// Log attack
								UE_LOG(LogTemp, Warning, TEXT("HitActorName: %s"), *Hit.Actor->GetName());
							}
						}
					}
					return true;
				}
			}
		}
	}
	
	return false;
}

/*___________________________
	Particle effect functions
*/
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

/*___________________________
	Get set functions
*/
void ABaseAbility::SetCurrentUser(AActor * user)
{
	currentUser = user;
}