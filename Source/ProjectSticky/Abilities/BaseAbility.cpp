// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAbility.h"
#include "Engine.h"


ABaseAbility::ABaseAbility()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	PrimaryActorTick.bCanEverTick = true;

}

void ABaseAbility::BeginPlay()
{
	
}

bool ABaseAbility::DamageActor(AActor * actor)
{
	return false;
}

void ABaseAbility::ChargeAbility_Implementation(AActor * user, FVector direction, float range)
{

}
bool ABaseAbility::ChargeAbility_Validate(AActor * user, FVector direction, float range)
{
	return true;
}

void ABaseAbility::ServerChargeAbility_Implementation(AActor * user, FVector direction, float range)
{

}

void ABaseAbility::ExecuteAbility_Implementation(AActor * user, FVector direction, float range)
{
	
}
bool ABaseAbility::ExecuteAbility_Validate(AActor * user, FVector direction, float range)
{
	return true;
}

void ABaseAbility::ServerExecuteAbility_Implementation(AActor * user, FVector direction, float range)
{

}