// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCorpse.h"


// Sets default values
ABaseCorpse::ABaseCorpse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	corpseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//corpseMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

}

// Called when the game starts or when spawned
void ABaseCorpse::BeginPlay()
{
	Super::BeginPlay();
	
}

ACharacter* ABaseCorpse::ReviveCharacter()
{
	if (referenceToDeadChar != nullptr)
	{
		return referenceToDeadChar;
	}

	return nullptr;
}

