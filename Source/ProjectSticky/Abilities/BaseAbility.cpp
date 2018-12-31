// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAbility.h"


ABaseAbility::ABaseAbility()
{

	PrimaryActorTick.bCanEverTick = true;

}
void ABaseAbility::BeginPlay()
{
}

void ABaseAbility::ChargeAbility(ABaseCharacter * user, FVector direction)
{

}

void ABaseAbility::ExecuteAbility(ABaseCharacter * user, FVector direction)
{

}
