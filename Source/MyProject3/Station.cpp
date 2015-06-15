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
#include "Math/RandomStream.h"
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

	Random.GenerateNewSeed();
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

	// This seems to be duplicating instances when used in a blueprint
	// that calls GenerateRandomMap in the constructor.
/*
	Random.Reset();
	ClearInstances();
	CubalMap.Empty(Cubal.Num());
	for (auto it = Cubal.CreateIterator(); it; ++it)
	{
		CubalMap.Add(it->Index, it.GetIndex());
		PlaceCubal(it.GetIndex());
	}
*/
}


void AStation::ClearInstances()
{
	FloorComponent->ClearInstances();
	FloorInstance.Empty();
	CeilingComponent->ClearInstances();
	CeilingInstance.Empty();
	WallComponent->ClearInstances();
	WallInstance.Empty();
	DoorwayComponent->ClearInstances();
	DoorwayInstance.Empty();
	WindowComponent->ClearInstances();
	WindowInstance.Empty();

	for (auto it = Cubal.CreateIterator(); it; ++it)
	{
		it->FloorInstance = -1;
		it->CeilingInstance = -1;
		it->LeftWallInstance = -1;
		it->BackWallInstance = -1;
		it->RightWallInstance = -1;
		it->FrontWallInstance = -1;
	}
}


int32 AStation::FindOrAddCubal(int32 X, int32 Y, int32 Z)
{
	FCubalIndex Key(X, Y, Z);
	int32* CubalIndex = CubalMap.Find(Key);

	CubalIndex = CubalMap.Find(Key);
	if (CubalIndex == nullptr)
	{
		int32 i = Cubal.Add(FCubal(X, Y, Z));
		CubalMap.Add(Key, i);
		return i;
	}

	return *CubalIndex;
}


void AStation::PlaceFloor_Implementation(EFloorId Id, int32 X, int32 Y, int32 Z)
{
	int32 LocalCubalIndex = FindOrAddCubal(X, Y, Z);
	FCubal& LocalCubal = Cubal[LocalCubalIndex];

	if (LocalCubal.FloorInstance >= 0)
	{
		FloorComponent->RemoveInstance(LocalCubal.FloorInstance);
		FloorInstance.RemoveSwap(LocalCubal.FloorInstance);
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
		FloorInstance.Add(LocalCubalIndex);
	}
}


bool AStation::PlaceFloor_Validate(EFloorId Id, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceCeiling_Implementation(ECeilingId Id, int32 X, int32 Y, int32 Z)
{
	int32 LocalCubalIndex = FindOrAddCubal(X, Y, Z);
	FCubal& LocalCubal = Cubal[LocalCubalIndex];

	if (LocalCubal.CeilingInstance >= 0)
	{
		CeilingComponent->RemoveInstance(LocalCubal.CeilingInstance);
		CeilingInstance.RemoveSwap(LocalCubal.CeilingInstance);
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
		CeilingInstance.Add(LocalCubalIndex);
	}
}


bool AStation::PlaceCeiling_Validate(ECeilingId Id, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceWall_Implementation(EWallDirection Direction, EWallId Id, int32 X, int32 Y, int32 Z)
{
	FTransform Transform;
	int32 LocalCubalIndex = FindOrAddCubal(X, Y, Z);
	FCubal& LocalCubal = Cubal[LocalCubalIndex];

	if (LocalCubal.WallInstance(Direction) >= 0)
	{
		switch (LocalCubal.Wall(Direction))
		{
		case EWallId::Wall:
			WallComponent->RemoveInstance(LocalCubal.WallInstance(Direction));
			WallInstance.RemoveSwap(LocalCubal.WallInstance(Direction));
			LocalCubal.SetWallInstance(Direction, -1);
			break;
		case EWallId::Doorway:
			DoorwayComponent->RemoveInstance(LocalCubal.WallInstance(Direction));
			DoorwayInstance.RemoveSwap(LocalCubal.WallInstance(Direction));
			LocalCubal.SetWallInstance(Direction, -1);
			break;
		case EWallId::Window:
			WindowComponent->RemoveInstance(LocalCubal.WallInstance(Direction));
			WindowInstance.RemoveSwap(LocalCubal.WallInstance(Direction));
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
		WallInstance.Add(LocalCubalIndex);
		break;
	case EWallId::Doorway:
		LocalCubal.SetWall(Direction, Id);
		LocalCubal.SetWallInstance(Direction, DoorwayComponent->AddInstance(Transform));
		DoorwayInstance.Add(LocalCubalIndex);
		break;
	case EWallId::Window:
		LocalCubal.SetWall(Direction, Id);
		LocalCubal.SetWallInstance(Direction, WindowComponent->AddInstance(Transform));
		WindowInstance.Add(LocalCubalIndex);
		break;
	}
}


bool AStation::PlaceWall_Validate(EWallDirection Direction, EWallId Id, int32 X, int32 Y, int32 Z)
{
	return true;
}


void AStation::PlaceCubal(int32 CubalIndex)
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


void AStation::GenerateRandomMap(int32 X, int32 Y, int32 Z, int32 Rooms)
{
	if (Rooms < 1) return;
	if (X < 4) X = 4;
	if (Y < 4) Y = 4;
	if (Z < 1) Z = 1;

	int32 MaxX = X * Rooms / 2;
	int32 MaxY = Y * Rooms / 2;
	TArray<bool> Map;
	Map.AddZeroed(MaxX * MaxY * Z);
	auto Idx = [MaxY, Z](int x, int y, int z) { return x * MaxY * Z + y * Z + z; };

	//I don't really want to reset things here, but there is weird duplicating
	//of data with a blueprint
	ClearInstances();
	Cubal.Empty();
	CubalMap.Empty();

	for (int32 i = 0; i < Rooms; ++i)
	{
		int32 Width = Random.RandRange(3, X);
		int32 Length = Random.RandRange(3, Y);
		int32 Height = Random.RandRange(1, Z);
		int32 XOff = Random.RandRange(0, MaxX - Width - 1);
		int32 YOff = Random.RandRange(0, MaxY - Length - 1);

		for (int32 j = XOff; j < XOff + Width; ++j)
		{
			for (int32 k = YOff; k < YOff + Length; ++k)
			{
				for (int32 l = 0; l < Height; ++l)
				{
					Map[Idx(j, k, l)] = true;
				}
			}
		}
	}

	for (int32 i = 0; i < MaxX; ++i)
	{
		for (int32 j = 0; j < MaxY; ++j)
		{
			for (int32 k = 0; k < Z; ++k)
			{
				if (Map[Idx(i, j, k)])
				{
					if (i == 0 || !Map[Idx(i-1,j,k)])
					{
						PlaceWall(EWallDirection::Right, EWallId::Wall, i, j, k);
					}
					if (i == MaxX - 1 || !Map[Idx(i+1,j,k)])
					{
						PlaceWall(EWallDirection::Left, EWallId::Wall, i, j, k);
					}
					if (j == 0 || !Map[Idx(i,j-1,k)])
					{
						PlaceWall(EWallDirection::Back, EWallId::Wall, i, j, k);
					}
					if (j == MaxY - 1 || !Map[Idx(i,j+1,k)])
					{
						PlaceWall(EWallDirection::Front, EWallId::Wall, i, j, k);
					}
					if (k == 0 || !Map[Idx(i,j,k-1)])
					{
						PlaceFloor(EFloorId::Floor, i, j, k);
					}
					if (k == Z - 1 || !Map[Idx(i,j,k+1)])
					{
						PlaceCeiling(ECeilingId::Ceiling, i, j, k);
					}
				}
			}
		}
	}
}

/* vim: set noexpandtab: */
