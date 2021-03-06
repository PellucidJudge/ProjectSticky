// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "BaseRoom.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	TT_Floor	UMETA(DisplayName = "Floor"),
	TT_Wall		UMETA(DisplayName = "Wall"),
	TT_Corner	UMETA(DisplayName = "Corner"),
	TT_Gate		UMETA(DisplayName = "Gate")
};

USTRUCT()
struct FTileStruct
{
	GENERATED_BODY()

	UPROPERTY()
	ETileType tileType;

	UPROPERTY()
	UStaticMeshComponent* mainMesh;

};

USTRUCT(BlueprintType)
struct FRoomStyle : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Descriptor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> AssetPaths;
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

	// Predefined components
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* root;
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* folderBaseMeshes;
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* testComponent;

	// Size settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomSettings")
	int32 roomSizeX = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomSettings")
	int32 roomSizeY = 3;

	// Overrides the arrays in this script in favor of the overall map style
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	bool useMapStyle = false;
	// Arrays Containing the style (intended to be switched out with datatables later)
	UPROPERTY(EditAnywhere, Category = "RoomStyle")
	UDataTable* roomStyleDataTable;

	// TEMPORARY "DATATABLE" for the meshes
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	UStaticMesh* floorMeshAsset;
	//FString floorMeshAsset = "/Game/Art/Meshes/PlaceHolder/PlaceHolderDungeon_Floor.PlaceHolderDungeon_Floor";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	UStaticMesh* wallMeshAsset;
	//FString wallMeshAsset = "/Game/Art/Meshes/PlaceHolder/PlaceHolderDungeon_Wall.PlaceHolderDungeon_Wall";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoomStyle")
	UStaticMesh* cornerMeshAsset;
	//FString cornerMeshAsset = "/Game/Art/Meshes/PlaceHolder/PlaceHolderDungeon_Corner.PlaceHolderDungeon_Corner";

	UPROPERTY()
	TArray<FTileStruct> tileGrid;

	UFUNCTION()
	UStaticMesh* GetRandomMeshInArray(TArray<UStaticMesh*> meshArray);
	UFUNCTION()
	UStaticMeshComponent* GetTileMesh(int32 XIndex, int32 YIndex);
	UFUNCTION()
	ETileType GetTileType(int32 XIndex, int32 YIndex);
	UFUNCTION()
	void SetRoomTileMesh(int32 XIndex, int32 YIndex, UStaticMesh* meshAsset);
	UFUNCTION()
	void SetRoomTileType(int32 XIndex, int32 YIndex, ETileType tileType);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
