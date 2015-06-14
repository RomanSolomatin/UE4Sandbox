/*
 * Copyright (c) 2015 Coy Barnes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "MyProject3.h"
#include "Station.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

AStation::AStation()
	: Super()
{
	bCanBeDamaged = false;
	bReplicates = true;
	FloorMesh = 0;
	CeilingMesh = 0;
	WallMesh = 0;
	DoorwayMesh = 0;
	WindowMesh = 0;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	FloorComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Floor"));
	CeilingComponent =  CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Ceiling"));
	WallComponent =  CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Wall"));
	DoorwayComponent =  CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Doorway"));
	WindowComponent =  CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Window"));

	RootComponent = Root;
	FloorComponent->AttachTo(RootComponent);
	CeilingComponent->AttachTo(RootComponent);
	WallComponent->AttachTo(RootComponent);
	DoorwayComponent->AttachTo(RootComponent);
	WindowComponent->AttachTo(RootComponent);
}


void AStation::BeginPlay()
{
	Super::BeginPlay();
}

	
void AStation::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AStation::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FloorComponent->SetStaticMesh(FloorMesh);
	CeilingComponent->SetStaticMesh(CeilingMesh);
	WallComponent->SetStaticMesh(WallMesh);
	DoorwayComponent->SetStaticMesh(DoorwayMesh);
	WindowComponent->SetStaticMesh(WindowMesh);
}


void AStation::PlaceFloor_Implementation(ETileId type, int32 X, int32 Y, int32 Z)
{
	FTransform Transform(FRotator(0, 0, 0),
	                     FVector(X * TileSize.X,
	                             Y * TileSize.Y,
	                             Z * TileSize.Z),
	                     FVector(1.0, 1.0, 1.0));

	/* TODO: Add sanity checks */
	Map.Deck[Z].Row[Y].Room[X].Floor = type;
	FloorComponent->AddInstance(Transform);
}


bool AStation::PlaceFloor_Validate(ETileId type, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceCeiling_Implementation(ETileId type, int32 X, int32 Y, int32 Z)
{
	FTransform Transform(FRotator(0, 0, 0),
	                     FVector(X * TileSize.X,
	                             Y * TileSize.Y,
	                             Z * TileSize.Z),
	                     FVector(1.0, 1.0, 1.0));

	/* TODO: Add sanity checks */
	Map.Deck[Z].Row[Y].Room[X].Ceiling = type;
	CeilingComponent->AddInstance(Transform);
}


bool AStation::PlaceCeiling_Validate(ETileId type, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceLeftWall_Implementation(ETileId type, int32 X, int32 Y, int32 Z)
{
	FTransform Transform(FRotator(0, 90, 0),
	                     FVector(X * TileSize.X + TileSize.X,
	                             Y * TileSize.Y,
	                             Z * TileSize.Z),
	                     FVector(1.0, 1.0, 1.0));

	/* TODO: Add sanity checks */
	Map.Deck[Z].Row[Y].Room[X].LeftWall = type;
	WallComponent->AddInstance(Transform);
}


bool AStation::PlaceLeftWall_Validate(ETileId type, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceRightWall_Implementation(ETileId type, int32 X, int32 Y, int32 Z)
{
	FTransform Transform(FRotator(0, -90, 0),
	                     FVector(X * TileSize.X,
	                             Y * TileSize.Y + TileSize.Y,
	                             Z * TileSize.Z),
	                     FVector(1.0, 1.0, 1.0));

	/* TODO: Add sanity checks */
	Map.Deck[Z].Row[Y].Room[X].RightWall = type;
	WallComponent->AddInstance(Transform);
}


bool AStation::PlaceRightWall_Validate(ETileId type, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceFrontWall_Implementation(ETileId type, int32 X, int32 Y, int32 Z)
{
	FTransform Transform(FRotator(0, 180, 0),
	                     FVector(X * TileSize.X + TileSize.X,
	                             Y * TileSize.Y + TileSize.Y,
	                             Z * TileSize.Z),
	                     FVector(1.0, 1.0, 1.0));

	/* TODO: Add sanity checks */
	Map.Deck[Z].Row[Y].Room[X].FrontWall = type;
	WallComponent->AddInstance(Transform);
}


bool AStation::PlaceFrontWall_Validate(ETileId type, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceBackWall_Implementation(ETileId type, int32 X, int32 Y, int32 Z)
{
	FTransform Transform(FRotator(0, 0, 0),
	                     FVector(X * TileSize.X,
	                             Y * TileSize.Y,
	                             Z * TileSize.Z),
	                     FVector(1.0, 1.0, 1.0));


	/* TODO: Add sanity checks */
	Map.Deck[Z].Row[Y].Room[X].BackWall = type;
	WallComponent->AddInstance(Transform);
}


bool AStation::PlaceBackWall_Validate(ETileId type, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::GenerateRandomMap(int32 X, int32 Y, int32 Z)
{
	Map.Setup(Z, Y, X);

	for (int i = 0; i < X; ++i)
	{
		for (int j = 0; j < Y; ++j)
		{
			PlaceFloor(ETileId::Floor, i, j, 0);
			PlaceCeiling(ETileId::Ceiling, i, j, Z - 1);
		}

		for (int j = 0; j < Z; ++j)
		{
			PlaceBackWall(ETileId::Wall, i, 0, j);
			PlaceFrontWall(ETileId::Wall, i, Y - 1, j);
		}
	}

	for (int i = 0; i < Y; ++i)
	{
		for (int j = 0; j < Z; ++j)
		{
			PlaceLeftWall(ETileId::Wall, 0, i, j);
			PlaceRightWall(ETileId::Wall, X - 1, i, j);
		}
	}
}

/* vim: set noexpandtab: */
