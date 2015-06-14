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


uint32 GetTypeHash(const FCubalIndex& A)
{
	// Nothing special just grabbed some random primes with help of wolfram alpha
	return (A.X * 0x1cc009bb + A.Y * 0x2c9af989 + A.Z * 0x135d9c09) % 0x50395aab;
}


AStation::AStation()
	: Super()
{
	bCanBeDamaged = false;
	bReplicates = true;
	FloorMesh = nullptr;
	CeilingMesh = nullptr;
	WallMesh = nullptr;
	DoorwayMesh = nullptr;
	WindowMesh = nullptr;

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


FCubal& AStation::FindOrAddCubal(int32 X, int32 Y, int32 Z)
{
	FCubalIndex Key(X, Y, Z);
	int32* CubalIndex = CubalMap.Find(Key);

	CubalIndex = CubalMap.Find(Key);
	if (CubalIndex == nullptr)
	{
		return Cubal[(CubalMap.Add(Key, Cubal.Add(FCubal(X, Y, Z))))];
	}

	return Cubal[*CubalIndex];
}


void AStation::PlaceFloor_Implementation(EFloorId Id, int32 X, int32 Y, int32 Z)
{
	FCubal& LocalCubal = FindOrAddCubal(X, Y, Z);

	if (LocalCubal.FloorInstance >= 0)
	{
		FloorComponent->RemoveInstance(LocalCubal.FloorInstance);
		LocalCubal.FloorInstance = -1;
	}

	if (Id == EFloorId::Floor)
	{
		FTransform Transform(FRotator(0, 0, 0),
		                     FVector(X * CubalSize.X,
		                             Y * CubalSize.Y,
		                             Z * CubalSize.Z),
		                     FVector(1.0, 1.0, 1.0));

		LocalCubal.Floor = Id;
		LocalCubal.FloorInstance = FloorComponent->AddInstance(Transform);
	}
}


bool AStation::PlaceFloor_Validate(EFloorId Id, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceCeiling_Implementation(ECeilingId Id, int32 X, int32 Y, int32 Z)
{
	FCubal& LocalCubal = FindOrAddCubal(X, Y, Z);

	if (LocalCubal.CeilingInstance >= 0)
	{
		CeilingComponent->RemoveInstance(LocalCubal.CeilingInstance);
		LocalCubal.CeilingInstance = -1;
	}

	if (Id == ECeilingId::Ceiling)
	{
		FTransform Transform(FRotator(0, 0, 0),
		                     FVector(X * CubalSize.X,
		                             Y * CubalSize.Y,
		                             Z * CubalSize.Z),
		                     FVector(1.0, 1.0, 1.0));

		LocalCubal.Ceiling = Id;
		LocalCubal.CeilingInstance = CeilingComponent->AddInstance(Transform);
	}
}


bool AStation::PlaceCeiling_Validate(ECeilingId Id, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceWall_Implementation(EWallDirection Direction, EWallId Id, int32 X, int32 Y, int32 Z)
{
	FTransform Transform;
	FCubal& LocalCubal = FindOrAddCubal(X, Y, Z);

	if (LocalCubal.WallInstance(Direction) >= 0)
	{
		switch (LocalCubal.Wall(Direction))
		{
		case EWallId::Wall:
			WallComponent->RemoveInstance(LocalCubal.WallInstance(Direction));
			LocalCubal.SetWallInstance(Direction, -1);
			break;
		case EWallId::Doorway:
			DoorwayComponent->RemoveInstance(LocalCubal.WallInstance(Direction));
			LocalCubal.SetWallInstance(Direction, -1);
			break;
		case EWallId::Window:
			WindowComponent->RemoveInstance(LocalCubal.WallInstance(Direction));
			LocalCubal.SetWallInstance(Direction, -1);
			break;
		}
	}

	switch (Direction)
	{
	case EWallDirection::Left:
		Transform = FTransform(FRotator(0, 90, 0),
		                       FVector((X + 1) * CubalSize.X,
		                               Y * CubalSize.Y,
		                               Z * CubalSize.Z),
		                       FVector(1.0, 1.0, 1.0));
		break;
	case EWallDirection::Right:
		Transform = FTransform(FRotator(0, -90, 0),
		                       FVector(X * CubalSize.X,
		                               (Y + 1) * CubalSize.Y,
		                               Z * CubalSize.Z),
		                       FVector(1.0, 1.0, 1.0));
		break;
	case EWallDirection::Back:
		Transform = FTransform(FRotator(0, 0, 0),
		                       FVector(X * CubalSize.X,
		                               Y * CubalSize.Y,
		                               Z * CubalSize.Z),
		                       FVector(1.0, 1.0, 1.0));
		break;
	case EWallDirection::Front:
		Transform = FTransform(FRotator(0, 180, 0),
		                       FVector((X + 1) * CubalSize.X,
		                               (Y + 1) * CubalSize.Y,
		                               Z * CubalSize.Z),
		                       FVector(1.0, 1.0, 1.0));
		break;
	}

	switch (Id)
	{
	case EWallId::Wall:
		LocalCubal.SetWall(Direction, Id);
		LocalCubal.SetWallInstance(Direction, WallComponent->AddInstance(Transform));
		break;
	case EWallId::Doorway:
		LocalCubal.SetWall(Direction, Id);
		LocalCubal.SetWallInstance(Direction, DoorwayComponent->AddInstance(Transform));
		break;
	case EWallId::Window:
		LocalCubal.SetWall(Direction, Id);
		LocalCubal.SetWallInstance(Direction, WindowComponent->AddInstance(Transform));
		break;
	}
}


bool AStation::PlaceWall_Validate(EWallDirection Direction, EWallId Id, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::RebuildCubal(int32 CubalIndex)
{
	if (CubalIndex < 0 || CubalIndex >= Cubal.Num())
	{
		return;
	}

	FCubal &LocalCubal = Cubal[CubalIndex];

	PlaceFloor(LocalCubal.Floor, LocalCubal.Index.X,
		   LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceCeiling(LocalCubal.Ceiling, LocalCubal.Index.X,
		     LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Back, LocalCubal.BackWall,
		  LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Front, LocalCubal.FrontWall,
		  LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Left, LocalCubal.LeftWall,
		  LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Right, LocalCubal.RightWall,
		  LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
}


void AStation::GenerateRandomMap(int32 X, int32 Y, int32 Z)
{
	for (int i = 0; i < X; ++i)
	{
		for (int j = 0; j < Y; ++j)
		{
			PlaceFloor(EFloorId::Floor, i, j, 0);
			PlaceCeiling(ECeilingId::Ceiling, i, j, Z - 1);
		}

		for (int j = 0; j < Z; ++j)
		{
			PlaceWall(EWallDirection::Back, EWallId::Wall, i, 0, j);
			PlaceWall(EWallDirection::Front, EWallId::Wall, i, Y - 1, j);
		}
	}

	for (int i = 0; i < Y; ++i)
	{
		for (int j = 0; j < Z; ++j)
		{
			PlaceWall(EWallDirection::Left, EWallId::Wall, X - 1, i, j);
			PlaceWall(EWallDirection::Right, EWallId::Wall, 0, i, j);
		}
	}
}

/* vim: set noexpandtab: */
