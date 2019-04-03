// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAbility.h"
#include "Engine.h"


AMeleeAbility::AMeleeAbility() : Super()
{

}

void AMeleeAbility::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (isCharging == true)
	{
		if (PSC_ChargeParticles != nullptr && currentUser != nullptr)
		{
			PSC_ChargeParticles->SetWorldLocation(currentUser->GetActorLocation());
		}
	}
}

void AMeleeAbility::Server_ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	Multi_ChargeAbility(user, direction, mouseLocation);
}

void AMeleeAbility::Multi_ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	isCharging = true;
	currentUser = user;
	direction = FVector(direction.X, direction.Y, 0);

	UWorld* world = GetWorld();
	if (world != nullptr && user != nullptr)
	{
		if (PS_AbilityCharge != nullptr)
		{
			FTransform spawnTransform;
			spawnTransform.SetLocation(user->GetActorLocation());
			spawnTransform.SetRotation(direction.ToOrientationQuat());
			spawnTransform.SetScale3D(FVector(1, 1, 1));
			
			if (PSC_ChargeParticles == nullptr)
			{
				PSC_ChargeParticles = SpawnLastingParticleEffect(PS_AbilityCharge, spawnTransform);
			}
			else
			{
				PSC_ChargeParticles->SetWorldTransform(spawnTransform);
				PSC_ChargeParticles->SetVisibility(true);
				PSC_ChargeParticles->Activate(true);
			}
		}
	}
}

// Called on clients to tell the server
void AMeleeAbility::Server_ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	if (GEngine)
	{
		if (Role == ROLE_Authority)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("SERVER"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("CLIENT"));
		}
	}

    Multi_ExecuteAbility(user, direction, mouseLocation);
}
// Called on server to forward information and actually execute the ability 
void AMeleeAbility::Multi_ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	isCharging = false;
	direction = FVector(direction.X, direction.Y, 0);

	if (GEngine)
	{
		if (Role == ROLE_Authority)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("SERVER"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, TEXT("CLIENT"));
		}
	}

	// Disable charge particle effect
	if (PSC_ChargeParticles != nullptr)
	{
		PSC_ChargeParticles->SetActive(false);
		PSC_ChargeParticles->SetVisibility(false);
	}

	if (user != nullptr)
	{
		FVector startPoint = user->GetActorLocation();

		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			// Play particle effect
			if (PS_AbilityExecutionPE != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, TEXT("PS_AbilityExecutionPE"));
				FTransform spawnTransform;
				spawnTransform.SetLocation(user->GetActorLocation() - FVector(0,0,100));
				spawnTransform.SetRotation(direction.ToOrientationQuat());
				spawnTransform.SetScale3D(FVector(1, 1, 1));

				SpawnParticleEffect(PS_AbilityExecutionPE, spawnTransform);
			}

			// Hit detection
			TArray<FHitResult> outHits;
			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
			FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("BoxTrace")), true, user);
			FCollisionShape colBox = FCollisionShape::MakeBox(FVector(50, 50, 50));

			DrawDebugBox(world, startPoint, FVector(50, 50, 50), FColor::Purple, false, 2.0f);
			DrawDebugBox(world, startPoint + (direction * baseRange), FVector(50, 50, 50), FColor::Purple, false, 2.0f);

			bool anyHit = world->SweepMultiByObjectType(outHits, startPoint, startPoint + (direction * baseRange),
				direction.Rotation().Quaternion(), ObjectParams, colBox, Params);

			if (anyHit)
			{
				for (auto& Hit : outHits)
				{
					if (Hit.Actor != nullptr)
					{
						AActor* hitActor = Cast<AActor>(Hit.Actor);
						IHealthManipulation* HMInterface = Cast<IHealthManipulation>(Hit.Actor);

						if (HMInterface != nullptr && hitActor != nullptr)
						{
							HMInterface->Execute_DamageObject(hitActor, abilityDamage, user, 100, direction);

							UE_LOG(LogTemp, Warning, TEXT("HitActorName: %s"), *Hit.Actor->GetName());
						}
					}
				}
			}
		}
	}
}