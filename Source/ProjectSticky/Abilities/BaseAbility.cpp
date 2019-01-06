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
	
}

bool ABaseAbility::DamageActor(AActor * actor)
{
	return false;
}
// Called on clients to tell the server
// This is called from the controlling character when an ability is charged
void ABaseAbility::ChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{

}
// Validate that the call is correct
bool ABaseAbility::ChargeAbility_Validate(AActor * user, FVector direction, FVector mouseLocation)
{
	return true;
}
// Called on server to forward information and actually execute the ability
void ABaseAbility::ServerChargeAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{

}
// Called on clients to tell the server
// Called from the controlling character when the attack is supposed to be executed
void ABaseAbility::ExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{
	
}
// Validate that the call is correct
bool ABaseAbility::ExecuteAbility_Validate(AActor * user, FVector direction, FVector mouseLocation)
{
	return true;
}
// Called on server to forward information and actually execute the ability
void ABaseAbility::ServerExecuteAbility_Implementation(AActor * user, FVector direction, FVector mouseLocation)
{

}
