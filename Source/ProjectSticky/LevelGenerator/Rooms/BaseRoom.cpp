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

	/*
	//TestStuff
	testComponent = CreateAbstractDefaultSubobject<UStaticMeshComponent>("test");
	testComponent->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetIncludingScale);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> testComponentAsset(TEXT("/Game/Art/Meshes/PlaceHolder/PlaceHolderDungeon_Floor.PlaceHolderDungeon_Floor"));
	if (testComponentAsset.Succeeded())
	{
		testComponent->SetStaticMesh(testComponentAsset.Object);
	}
	*/

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

	// Set mesh types
	for (int32 XIndex = 0; XIndex < roomSizeX; XIndex++)
	{
		for (int32 YIndex = 0; YIndex < roomSizeY; YIndex++)
		{

			static ConstructorHelpers::FObjectFinder<UStaticMesh> testComponentAsset(TEXT("/Game/Art/Meshes/PlaceHolder/PlaceHolderDungeon_Floor.PlaceHolderDungeon_Floor"));
			if (testComponentAsset.Succeeded() && tileGrid.Num() > 0)
			{
				tileGrid[XIndex + roomSizeX * YIndex].mainMesh->SetStaticMesh(testComponentAsset.Object);
			}

			if (roomMeshes_Floors.Num() > 0)
			{
				SetRoomTileMesh(XIndex, YIndex, GetRandomMeshInArray(roomMeshes_Floors));
			}
			
			// Set mesh
			switch (GetTileType(XIndex,YIndex))
			{
			case ETileType::TT_Floor :
				break;

			case ETileType::TT_Wall :
				break;

			case ETileType::TT_Corner :
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
	/*
	UE_LOG(LogTemp, Warning, TEXT("Hej"));

	if (roomMeshes_Floors.Num() > 0)
	{
		if (roomMeshes_Floors[0] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), tileGrid.Num());
		}
		
	}
	*/
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

void ABaseRoom::SetRoomTileMesh(int32 XIndex, int32 YIndex, UStaticMesh * mesh)
{
	tileGrid[XIndex + roomSizeX * YIndex].mainMesh->SetStaticMesh(mesh);
}

void ABaseRoom::SetRoomTileType(int32 XIndex, int32 YIndex, ETileType tileType)
{
	tileGrid[XIndex + roomSizeX * YIndex].tileType = tileType;
}

