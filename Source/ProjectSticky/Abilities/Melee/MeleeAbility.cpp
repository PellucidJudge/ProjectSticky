// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAbility.h"
#include "ProjectSticky/Interfaces/HealthManipulation.h"

void AMeleeAbility::ChargeAbility(AActor * user, FVector direction, float range)
{
	if (user != nullptr)
	{

	}
}

void AMeleeAbility::ExecuteAbility(AActor * user, FVector direction, float range)
{
	UE_LOG(LogTemp, Warning, TEXT("HIIIIIYA"));
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
			FCollisionShape colBox = FCollisionShape::MakeBox(FVector(50,50,50));

			DrawDebugBox(world, startPoint, FVector(50,50,50), FColor::Purple, false, 2.0f);
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