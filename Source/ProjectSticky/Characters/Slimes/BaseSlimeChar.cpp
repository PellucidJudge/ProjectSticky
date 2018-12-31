// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSlimeChar.h"

void ABaseSlimeChar::BeginPlay()
{
}

void ABaseSlimeChar::SetSlimeActive_Implementation(bool value)
{
	if (value == true)
	{
		SetActorEnableCollision(true);
		GetMesh()->SetVisibility(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetEnableGravity(true);
		this->SetIsChargingAttack(false);
	}
	else 
	{
		SetActorEnableCollision(false);
		GetMesh()->SetVisibility(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetEnableGravity(false);
		this->SetIsChargingAttack(false);
	}
}
