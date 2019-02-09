// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "UnrealNetwork.h"
#include "Engine.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	bReplicates = true;
	bReplicateMovement = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, movementSpeed);
	DOREPLIFETIME(ABaseCharacter, movementSpeedMod);
	//DOREPLIFETIME(ABaseCharacter, IsChargingAnAttack);
}


// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
	}
	*/
	SetHealthCurrent(healthStart);

	// Create ability instances
	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("beginplay"));
		if (BasicAbilityClass != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("create ability"));
			basicAbility = world->SpawnActor<ABaseAbility>(BasicAbilityClass);
			basicAbility->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			basicAbility->SetOwner(this);
		}
		if (SecondaryAbilityClass != nullptr)
		{
			secondaryAbility = world->SpawnActor<ABaseAbility>(SecondaryAbilityClass);
			secondaryAbility->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			secondaryAbility->SetOwner(this);
		}
		if (Slot1AbilityClass != nullptr)
		{
			slot1Ability = world->SpawnActor<ABaseAbility>(Slot1AbilityClass);
			slot1Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			slot1Ability->SetOwner(this);
		}
		if (Slot2AbilityClass != nullptr)
		{
			slot2Ability = world->SpawnActor<ABaseAbility>(Slot2AbilityClass);
			slot2Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			slot2Ability->SetOwner(this);
		}
		if (Slot3AbilityClass != nullptr)
		{
			slot3Ability = world->SpawnActor<ABaseAbility>(Slot3AbilityClass);
			slot3Ability->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			slot3Ability->SetOwner(this);
		}
		
	}

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the knockback if the character is currently being knocked back
	if (IsKnockedback)
	{
		KnockBack_Update(DeltaTime);
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::DamageObject_Implementation(TArray<FDamageStruct>& damage, AActor * attacker, float knockBackDistance, FVector knockBackDir)
{
	
	for (FDamageStruct damageStruct : damage)
	{
		float totalPhysicalDamage = 0;
		float totalFireDamage = 0;
		float totalColdDamage = 0;
		float totalElectricDamage = 0;
		float totalCorrosiveDamage = 0;

		// Calculate and apply the damage
		// Log the damage event
		switch (damageStruct.damageType)
		{
		// Physical follows rules as normal, destroys armor first and then health.
		// No additional effects in itself
		case EDamageTypes::DT_Physical:
			// Handle damage
			totalPhysicalDamage = damageStruct.damageAmount * defenceAndWeaknesess.physical_DamageMod;
			StandardDamage(totalPhysicalDamage);

			UE_LOG(LogTemp, Warning, TEXT("DAMAGE LOG: %s attacked %d for %f physicaldamage"), 
				*attacker->GetName(), *this->GetName(), damageStruct.damageAmount);
			break;

		// Fire damage behaves according to normal rules, armor first then health
		// Can have a chance to ignite the attacked but is handled seperately
		case EDamageTypes::DT_Fire:
			// Handle damage
			totalFireDamage = damageStruct.damageAmount * defenceAndWeaknesess.fire_DamageMod;
			StandardDamage(totalFireDamage);

			UE_LOG(LogTemp, Warning, TEXT("DAMAGE LOG: %s attacked %d for %f firedamage"), 
				*attacker->GetName(), *this->GetName(), damageStruct.damageAmount);
			break;

		// Cold damage behaves like normal when it comes to armor and health, armor first then health.
		// Cold applies frostbite and puts out fires
		case EDamageTypes::DT_Cold:
			// Handle damage
			totalColdDamage = damageStruct.damageAmount * defenceAndWeaknesess.cold_DamageMod;
			StandardDamage(totalColdDamage);

			UE_LOG(LogTemp, Warning, TEXT("DAMAGE LOG: %s attacked %d for %f colddamage"), 
				*attacker->GetName(), *this->GetName(), damageStruct.damageAmount);
			break;

		// Electric ignores armor and damages health directly
		// No other effects as default
		case EDamageTypes::DT_Electric:
			// Handle damage
			totalElectricDamage = damageStruct.damageAmount * defenceAndWeaknesess.electric_DamageMod;
			SetHealthCurrent(GetHealthCurrent() - damageStruct.damageAmount);

			UE_LOG(LogTemp, Warning, TEXT("DAMAGE LOG: %s attacked %d for %f electricdamage"), 
				*attacker->GetName(), *this->GetName(), damageStruct.damageAmount);
			break;

		// Corrosive damages armor first then health
		// Corrosive deals double damage towards armor
		case EDamageTypes::DT_Corrosive:
			// Handle damage
			totalCorrosiveDamage = damageStruct.damageAmount * defenceAndWeaknesess.corrosive_DamageMod;
			// Check if there is armor
			if (GetArmorCurrent() <= 0)
			{
				SetHealthCurrent(GetHealthCurrent() - totalCorrosiveDamage);
			}
			// Check if the armor will block the damage
			else if ((totalCorrosiveDamage * 2) <= GetArmorCurrent())
			{
				SetArmorCurrent(GetArmorCurrent() - (totalCorrosiveDamage *2));
			}
			else
			{
				float remainingHealthDamage = ((totalCorrosiveDamage * 2) - GetArmorCurrent()) / 2;
				SetHealthCurrent(GetHealthCurrent() - remainingHealthDamage);
			}

			UE_LOG(LogTemp, Warning, TEXT("DAMAGE LOG: %s attacked %d for %f corrosivedamage"), 
				*attacker->GetName(), *this->GetName(), damageStruct.damageAmount);
			break;

		default:
			UE_LOG(LogTemp, Error, TEXT("No corresponding damage type found"));
			break;
		}

		
		UE_LOG(LogTemp, Warning, TEXT("%s has %d hitpoints left out of a total %f hitpoints"), 
			*GetName(), GetHealthCurrent(), GetHealthMax());
	}
	
	if (GetHealthCurrent() <= 0)
	{
		CharDeath();
	}
	// Knockback
	else
	{
		if (IsKnockedback == false)
		{
			KnockBack_Start(knockBackDir, knockBackDistance);
		}
		else
		{

		}
	}
}

// Function for taking damage in the standard manner, armor first then health
void ABaseCharacter::StandardDamage(float damageAmount)
{
	// Check if there is armor
	if (GetArmorCurrent() <= 0)
	{
		SetHealthCurrent(GetHealthCurrent() - damageAmount);
	}
	// Check if the armor will block the damage
	else if (damageAmount <= GetArmorCurrent())
	{
		SetArmorCurrent(GetArmorCurrent() - damageAmount);
	}
	else
	{
		float remainingHealthDamage = damageAmount - GetArmorCurrent();
		SetHealthCurrent(GetHealthCurrent() - remainingHealthDamage);
	}
}

// Knockback functions
void ABaseCharacter::KnockBack_Start(FVector direction, float length)
{	
	IsKnockedback = true;

	knockbackDirection = direction;
	currentKnockbackLength = length;
	currentKnockbackProgress = 0;
}

void ABaseCharacter::KnockBack_Update(float deltaSeconds)
{
	if (currentKnockbackProgress >= currentKnockbackLength)
	{
		KnockBack_End();
	}
	else
	{
		currentKnockbackProgress = currentKnockbackProgress + (baseKnockbackSpeedPerSecond * deltaSeconds);

		float knockbackLength = (baseKnockbackSpeedPerSecond * deltaSeconds);
		FVector frameMovement = knockbackDirection * knockbackLength;
		frameMovement.Z = 0;

		this->SetActorLocation(GetActorLocation() + frameMovement, true);
	}
}

void ABaseCharacter::KnockBack_End()
{
	IsKnockedback = false;
}

// Called when health reaches 0
void ABaseCharacter::CharDeath()
{
	SetCharState(ECharState::CS_Dead);

	GetCharacterMovement()->GravityScale = 0;
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::MoveCharacter(FVector moveDir)
{
	if (currentCharState != ECharState::CS_Dead)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			if (moveDir.GetSafeNormal(1) != FVector(0, 0, 0))
			{
				moveDir.Normalize(1);
				AddMovementInput(moveDir, movementSpeedMod);
			}
		}
	}
}

void ABaseCharacter::UpdateLookingDirection(float rotation)
{
	if (rotateToFaceMoveDirection == false)
	{
		FRotator lookDirection = FRotator(0, 0, 0);
		lookDirection.Roll = this->GetActorRotation().Roll;
		lookDirection.Pitch = this->GetActorRotation().Pitch;
		lookDirection.Yaw = rotation;

		this->SetActorRotation(lookDirection);
	}
	
}

void ABaseCharacter::StartAttackCharge_Implementation(FVector attackDir, EAttackSlots attackSlotUsed, FVector mouseLocation)
{
	movementSpeedMod = 0.5;

	switch (attackSlotUsed)
	{
	case EAttackSlots::AS_BasicAttack:
		if (basicAbility != nullptr)
		{
			SetCharState(ECharState::CS_Charging);
			basicAbility->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_SecondaryAttack:
		if (secondaryAbility != nullptr)
		{
			SetCharState(ECharState::CS_Charging);
			secondaryAbility->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_Slot1:
		if (slot1Ability != nullptr)
		{
			SetCharState(ECharState::CS_Charging);
			slot1Ability->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_Slot2:
		if (slot2Ability != nullptr)
		{
			SetCharState(ECharState::CS_Charging);
			slot2Ability->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;

	case EAttackSlots::AS_Slot3:
		if (slot3Ability != nullptr)
		{
			SetCharState(ECharState::CS_Charging);
			slot3Ability->ChargeAbility(this, attackDir, mouseLocation);
		}
		break;
	}
}
void ABaseCharacter::InterruptAttackCharge_Implementation()
{
	movementSpeedMod = 1;
}
void ABaseCharacter::Attack_Implementation(FVector attackDir, EAttackSlots attackSlotUsed, FVector mouseLocation)
{
	movementSpeedMod = 1;

	switch (attackSlotUsed)
	{
	case EAttackSlots::AS_BasicAttack:
		if (GetCharState() == ECharState::CS_Charging)
		{
			if (basicAbility != nullptr)
			{
				basicAbility->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_SecondaryAttack:
		if (GetCharState() == ECharState::CS_Charging)
		{
			if (secondaryAbility != nullptr)
			{
				secondaryAbility->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_Slot1:
		if (GetCharState() == ECharState::CS_Charging)
		{
			if (slot1Ability != nullptr)
			{
				slot1Ability->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_Slot2:
		if (GetCharState() == ECharState::CS_Charging)
		{
			if (slot2Ability != nullptr)
			{
				slot2Ability->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;

	case EAttackSlots::AS_Slot3:
		if (GetCharState() == ECharState::CS_Charging)
		{
			if (slot3Ability != nullptr)
			{
				slot3Ability->ExecuteAbility(this, attackDir, mouseLocation);
			}
		}
		break;
	}

	SetCharState(ECharState::CS_Idle);
}


// Get and set functions
ECharState ABaseCharacter::GetCharState() { return currentCharState; }
void ABaseCharacter::SetCharState_Implementation(ECharState value) { currentCharState = value; }
bool ABaseCharacter::SetCharState_Validate(ECharState value) { return true; }

float ABaseCharacter::GetHealthCurrent() { return healthCurrent; }
void ABaseCharacter::SetHealthCurrent(float value){ healthCurrent = value; }

float ABaseCharacter::GetHealthMax() { return healthMax; }
void ABaseCharacter::SetHealthMax(float value) { healthMax = value; }

void ABaseCharacter::SetArmorCurrent(float value) { armorCurrent = value; }
float ABaseCharacter::GetArmorCurrent() { return armorCurrent; }
