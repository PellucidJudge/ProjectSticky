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
	}
}


void APointAndClickAbility::ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	ServerChargeAbility(user, direction, mouseLocation);
}

void APointAndClickAbility::ServerChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	isCharging = true;
	direction = FVector(direction.X, direction.Y, 0);
	startPos = mouseLocation;

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
	}

	psCurrentSizeProgression = 0;
}

// Called on clients to tell the server
void APointAndClickAbility::ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	ServerExecuteAbility(user, direction, mouseLocation);
}
// Called on server to forward information and actually execute the ability 
void APointAndClickAbility::ServerExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	isCharging = false;
	direction = FVector(direction.X, direction.Y, 0);

	if (chargeParticleSystem != nullptr)
	{
		chargeParticleSystem->SetActive(false);
		chargeParticleSystem->SetVisibility(false);
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
				spawnTransform.SetLocation(startPos);
				spawnTransform.SetRotation(direction.ToOrientationQuat());
				spawnTransform.SetScale3D(FVector(1, 1, 1));

				UGameplayStatics::SpawnEmitterAtLocation(world, PS_AbilityExecutionPE, spawnTransform, true);
			}

			// Hit detection
			TArray<FHitResult> outHits;
			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
			FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("BoxTrace")), true, user);
			FCollisionShape Sphere = FCollisionShape::MakeSphere(200);

			//DrawDebugSphere(world, mouseLocation, 200, 16, FColor::Green, false, 2.0f);

			bool anyHit = world->SweepMultiByObjectType(outHits, startPoint, startPoint + (direction * baseRange),
				direction.Rotation().Quaternion(), ObjectParams, Sphere, Params);

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
							//HMInterface->Execute_DamageObject(hitActor, damage, user, 100, direction);

							//UE_LOG(LogTemp, Warning, TEXT("HitActorName: %s") *Hit.Actor->GetName());
						}
					}
				}
			}
		}
	}
}

