// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HealthManipulationStructs.generated.h"

/**
 * 
 */


 /*
 *	All of the current damage types in the game
 */
UENUM(BlueprintType)
enum class EDamageTypes : uint8
{
	DT_Physical		UMETA(DisplayName = "Physical"),
	DT_Fire			UMETA(DisplayName = "Fire"),
	DT_Cold			UMETA(DisplayName = "Cold"),
	DT_Electric		UMETA(DisplayName = "Electric"),
	DT_Corrosive	UMETA(DisplayName = "Corrosive")
};

/*
*	All of the current status effects in the game
*/
UENUM(BlueprintType)
enum class EStatusEffects : uint8
{
	SE_Slow			UMETA(DisplayName = "Slow"),
	SE_Stun			UMETA(DisplayName = "Stun"),
	SE_Paralysis	UMETA(DisplayName = "Paralysis"),
	SE_Frostbite	UMETA(DisplayName = "Frostbite"),
	SE_Ignited		UMETA(DisplayName = "Ignited"),
	SE_Toxin		UMETA(DisplayName = "Toxin"),
	SE_Bleeding		UMETA(DisplayName = "Bleeding")
};

/*
*	Used for sending damage information from abilities to other actors
*/
USTRUCT(BlueprintType)
struct FDamageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageTypes damageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damageAmount;
};

/*
*	Struct will be used for base damage resistances of characters and objects
*	The modifiers here will either increase or decrease the incoming damage depending on type
*/
USTRUCT(BlueprintType)
struct FDefenceWeaknessStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Modifiers")
	float physical_DamageMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Modifiers")
	float fire_DamageMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Modifiers")
	float cold_DamageMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Modifiers")
	float electric_DamageMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Modifiers")
	float corrosive_DamageMod;
};

/*
*	Collection of status resistances and other status related data
*	Will be used to set defaults in characters
*/
USTRUCT(BlueprintType)
struct FDefendingStatuEffectsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	bool IsSlowImmune = false;
	// Slow resistance changes the strength of the slow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float slowResistanceMod;
	// Sets a max to the total effect all slow effects can have
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float slowMaxPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	bool IsStunImmune = false;
	// Modifies the length of the slow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	float stunResistanceMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paralysis")
	bool IsParalysisImmune = false;
	// Modifies the length of the paralysis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paralysis")
	float paralysisResistanceMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frostbite")
	bool IsFrostbiteImmune = false;
	// Modifies the length of the frostbite
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frostbite")
	float frostbiteResistanceMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ignited")
	bool IsIgnitedImmune = false;
	// Changes the chance of being ignited
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ignited")
	float ignitedResistanceChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toxin")
	bool IsToxinImmune = false;
	// Changes the max amount required to get the toxin effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toxin")
	float toxinResistanceMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toxin")
	bool IsBleedingImmune = false;
	// Changes the amount of bleed stacks needed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bleeding")
	float bleedingResistanceMod;
	
};

UCLASS()
class PROJECTSTICKY_API UHealthManipulationStructs : public UObject
{
	GENERATED_BODY()
	
	
	
	
};
