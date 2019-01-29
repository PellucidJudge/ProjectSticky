// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "ProjectSticky/Corpses/BaseCorpse.h"
#include "BaseEmenyChar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSTICKY_API ABaseEmenyChar : public ABaseCharacter
{
	GENERATED_BODY()
	
protected:

	UPROPERTY()
	bool IsControlleBySlime = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corpse")
	TSubclassOf<ABaseCorpse> enemyCorpse;
	
public:

	virtual void CharDeath() override;
};
