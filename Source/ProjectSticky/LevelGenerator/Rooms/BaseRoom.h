// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "BaseRoom.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	TT_Corner	UMETA(DisplayName = "Corner"),
	TT_Wall		UMETA(DisplayName = "Wall"),
	TT_Floor	UMETA(DisplayName = "Floor")
};

USTRUCT()
struct FTileStruct
{
	GENERATED_BODY()

		UPROPERTY()
		ETileType tileType;

	UPROPERTY()
		TArray<UStaticMesh*> props;

};

UCLASS()
class PROJECTSTICKY_API ABaseRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const float ROOMTILEWIDTH = 300;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomSettings")
	int32 roomSizeX = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomSettings")
	int32 roomSizeY = 3;

	// Overrides the arrays in this script in favor of the overall map style
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	bool useMapStyle = false;
	// Arrays Containing the style (intended to be switched out with datatables later)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	TArray<UStaticMesh*> roomMeshes_Corners;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	TArray<UStaticMesh*> roomMeshes_Walls;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	TArray<UStaticMesh*> roomMeshes_Floors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
