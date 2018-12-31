// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BaseSlimeChar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API ABaseSlimeChar : public ABaseCharacter
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Possession")
	void SetSlimeActive(bool value);

};
