// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BaseAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API ABaseAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	ABaseAICharacter();

	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree* AIBehavior;

private:
	
	virtual void BeginPlay() override;
	
};
