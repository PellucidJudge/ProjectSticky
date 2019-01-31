// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseRoom.h"


// Sets default values
ABaseRoom::ABaseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;
	folderBaseMeshes = CreateDefaultSubobject<USceneComponent>("FolderBaseMeshes");
	folderBaseMeshes->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetIncludingScale);

	// Create the array
	for (int32 XIndex = 0; XIndex < roomSizeX; XIndex++)
	{
		for (int32 YIndex = 0; YIndex < roomSizeY; YIndex++)
		{
			FTileStruct currentTile;

			// Create mesh components
			currentTile.mainMesh = CreateDefaultSubobject<UStaticMeshComponent>(*FString("Mesh" + FString::FromInt(XIndex + roomSizeX * YIndex)));
			currentTile.mainMesh->AttachToComponent(folderBaseMeshes, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// Set base value
			currentTile.tileType = ETileType::TT_Floor;

			tileGrid.Add(currentTile);
		}
	}

	// Identify what tile type each tile is
	for (int32 XIndex = 0; XIndex < roomSizeX; XIndex++)
	{
		for (int32 YIndex = 0; YIndex < roomSizeY; YIndex++)
		{
			// This currently assumes a rectangle room

			// First and last row walls and corners
			if (XIndex == 0 || XIndex == roomSizeX - 1)
			{
				if (YIndex == 0)
				{
					SetRoomTileType(XIndex, YIndex, ETileType::TT_Corner);
				}
				else if (YIndex == roomSizeY - 1)
				{
					SetRoomTileType(XIndex, YIndex, ETileType::TT_Corner);
				}
				else
				{
					SetRoomTileType(XIndex, YIndex, ETileType::TT_Wall);
				}
			}

			// Middle rows are walls and floors
			else
			{
				if (YIndex == 0)
				{
					SetRoomTileType(XIndex, YIndex, ETileType::TT_Wall);
				}
				else if (YIndex == roomSizeY - 1)
				{
					SetRoomTileType(XIndex, YIndex, ETileType::TT_Wall);
				}
				else
				{
					SetRoomTileType(XIndex, YIndex, ETileType::TT_Floor);
				}
			}
		}
	}


	// Set mesh types
	for (int32 XIndex = 0; XIndex < roomSizeX; XIndex++)
	{
		for (int32 YIndex = 0; YIndex < roomSizeY; YIndex++)
		{	
			// Set mesh
			/*
			if (roomStyleDataTable == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Tried to add data table"));
				static ConstructorHelpers::FObjectFinder<UDataTable> roomStyleDataTable(TEXT("DataTable'/Game/Misc/DataTables/DT_LvlStyle_Placeholder.DT_LvlStyle_Placeholder'"));
			}
			//	DataTable stuff
			
			static const FString ContextString(TEXT("GENERAL"));
			FRoomStyle* floorStruct = roomStyleDataTable->FindRow<FRoomStyle>(FName(TEXT("Floors")), ContextString, true);
			FRoomStyle* wallStruct = roomStyleDataTable->FindRow<FRoomStyle>(FName(TEXT("Walls")), ContextString, true);
			FRoomStyle* cornerStruct = roomStyleDataTable->FindRow<FRoomStyle>(FName(TEXT("Corners")), ContextString, true);
			*/
			
			
			switch (GetTileType(XIndex, YIndex))
			{
			case ETileType::TT_Floor:
				UE_LOG(LogTemp, Log, TEXT("FLOOR created at local room coords: %d,%d"), XIndex, YIndex);
				SetRoomTileMesh(XIndex, YIndex, floorMeshAsset);
				break;

			case ETileType::TT_Wall:
				UE_LOG(LogTemp, Log, TEXT("WALL created at local room coords: %d,%d"), XIndex, YIndex);
				SetRoomTileMesh(XIndex, YIndex, wallMeshAsset);
				break;

			case ETileType::TT_Corner:
				UE_LOG(LogTemp, Log, TEXT("CORNER created at local room coords: %d,%d"), XIndex, YIndex);
				SetRoomTileMesh(XIndex, YIndex, cornerMeshAsset);
				break;

			default:
				break;
			}
			
			//Set location
			FVector tileLocation = FVector(ROOMTILEWIDTH * XIndex, ROOMTILEWIDTH * YIndex, 0);
			tileGrid[XIndex + roomSizeX * YIndex].mainMesh->SetRelativeLocation(tileLocation);

		}
	}

}

// Called when the game starts or when spawned
void ABaseRoom::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMesh * ABaseRoom::GetRandomMeshInArray(TArray<UStaticMesh*> meshArray)
{
	UStaticMesh* returnMesh = nullptr;
	if (meshArray.Num() > 0)
	{
		returnMesh = meshArray[FMath::FloorToInt(FMath::FRandRange(0, meshArray.Num()))];
	}

	return returnMesh;
}


//Functions for accessing the array 1D array as an 2D array
UStaticMeshComponent * ABaseRoom::GetTileMesh(int32 XIndex, int32 YIndex)
{
	UStaticMeshComponent* returnMesh = tileGrid[XIndex + roomSizeX * YIndex].mainMesh;
	return returnMesh;
}

ETileType ABaseRoom::GetTileType(int32 XIndex, int32 YIndex)
{
	ETileType returnTileType = tileGrid[XIndex + roomSizeX * YIndex].tileType;
	return returnTileType;
}

void ABaseRoom::SetRoomTileMesh(int32 XIndex, int32 YIndex, FString meshAsset)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> floorMesh(*meshAsset);
	if (floorMesh.Succeeded())
	{
		int32 index = XIndex + roomSizeX * YIndex;
		tileGrid[index].mainMesh->SetStaticMesh(floorMesh.Object);
	}
	else UE_LOG(LogTemp, Warning, TEXT("No mesh found when generating room"));
}

void ABaseRoom::SetRoomTileType(int32 XIndex, int32 YIndex, ETileType tileType)
{
	tileGrid[XIndex + roomSizeX * YIndex].tileType = tileType;
}

