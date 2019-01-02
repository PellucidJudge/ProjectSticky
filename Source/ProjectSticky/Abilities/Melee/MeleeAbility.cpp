// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAbility.h"
#include "ProjectSticky/Interfaces/HealthManipulation.h"
#include "Engine.h"

void AMeleeAbility::ChargeAbility_Implementation(AActor * user, FVector direction, float range)
{
	if (user != nullptr)
	{

	}
}

void AMeleeAbility::ServerChargeAbility_Implementation(AActor * user, FVector direction, float range)
{
}

void AMeleeAbility::ExecuteAbility_Implementation(AActor * user, FVector direction, float range)
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
    ServerExecuteAbility(user, direction, range);
}
void AMeleeAbility::ServerExecuteAbility_Implementation(AActor * user, FVector direction, float range)
{

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

	if (user != nullptr)
	{
		FVector startPoint = user->GetActorLocation();

		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			TArray<FHitResult> outHits;
			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
			FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("BoxTrace")), true, user);
			FCollisionShape colBox = FCollisionShape::MakeBox(FVector(50, 50, 50));

			DrawDebugBox(world, startPoint, FVector(50, 50, 50), FColor::Purple, false, 2.0f);
			DrawDebugBox(world, startPoint + (direction * range), FVector(50, 50, 50), FColor::Purple, false, 2.0f);

			bool anyHit = world->SweepMultiByObjectType(outHits, startPoint, startPoint + (direction * range),
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
							HMInterface->Execute_DamageObject(hitActor, 5, user, 100, direction);

							//UE_LOG(LogTemp, Warning, TEXT("HitActorName: %s") *Hit.Actor->GetName());
						}
					}
				}
			}
		}
	}
}
/*
bool ABaseAbility::DamageActor(AActor * actor)
{
	return false;
}
*/