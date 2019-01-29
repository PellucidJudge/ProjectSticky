// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEmenyChar.h"


void ABaseEmenyChar::CharDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy has died"));

	GetCharacterMovement()->GravityScale = 0;
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (enemyCorpse != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid corpse"));
		UWorld* world = GetWorld();
		if (world)
		{
			UE_LOG(LogTemp, Warning, TEXT("A Corpse was spawned"));

			FVector location = this->GetActorLocation();
			FRotator rotation = this->GetActorRotation();
			FActorSpawnParameters SPawnInfo;
			world->SpawnActor<ABaseCorpse>(enemyCorpse, location, rotation, SPawnInfo);
		}
	}
}

