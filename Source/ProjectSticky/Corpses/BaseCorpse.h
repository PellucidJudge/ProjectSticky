// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "BaseCorpse.generated.h"

UCLASS()
class PROJECTSTICKY_API ABaseCorpse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCorpse();

	UFUNCTION()
	void SetCharacter(ACharacter* deadChar);
	UFUNCTION()
	ACharacter* ReviveCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* corpseMesh;
	UPROPERTY()
	ACharacter* referenceToDeadChar;
	
};
