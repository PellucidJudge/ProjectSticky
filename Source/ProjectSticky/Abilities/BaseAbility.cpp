// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAbility.h"


ABaseAbility::ABaseAbility()
{

	PrimaryActorTick.bCanEverTick = true;

}
void ABaseAbility::BeginPlay()
{
}

bool ABaseAbility::DamageActor(AActor * actor)
{
	return false;
}

void ABaseAbility::ChargeAbility(AActor * user, FVector direction, float range)
{

}

void ABaseAbility::ExecuteAbility(AActor * user, FVector direction, float range)
{

}
