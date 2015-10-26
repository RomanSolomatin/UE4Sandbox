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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	EmptyWall.Id = EWallId::None;
	Wall.Id = EWallId::Wall;
	Doorway.Id = EWallId::Doorway;
	Window.Id = EWallId::Window;
	Ceiling.Id = EWallId::Ceiling;

	Random.GenerateNewSeed();

	int count = 0;
	for (int i = 1; i < (int)EWallId::Max; ++i)
	{
		for (int j = (int)EWallComponentId::Single; j < (int)EWallComponentId::Max; ++j)
		{
			FWallComponent& LocalWall = FindWallComponentId((EWallId)i, (EWallComponentId)j);
			LocalWall.Hism = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(FName(TEXT("Component"), count));
			LocalWall.Hism->AttachTo(RootComponent);
		}
	}
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

	for (int i = 1; i < (int)EWallId::Max; ++i)
	{
		for (int j = (int)EWallComponentId::Single; j < (int)EWallComponentId::Max; ++j)
		{
			FWallComponent& LocalWall = FindWallComponentId((EWallId)i, (EWallComponentId)j);
			LocalWall.Hism->SetStaticMesh(LocalWall.Mesh);
		}
	}

}


void AStation::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}


void AStation::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void AStation::ClearInstances()
{
	for (int i = 1; i < (int)EWallId::Max; ++i)
	{
		for (int j = (int)EWallComponentId::Single; j < (int)EWallComponentId::Max; ++j)
		{
			FWallComponent& LocalWall = FindWallComponentId((EWallId)i, (EWallComponentId)j);
			LocalWall.Hism->ClearInstances();
			LocalWall.Instance.Empty();
		}
	}

	for (auto& C : Cubal)
	{
		C.FloorInstance = -1;
		C.CeilingInstance = -1;
		C.LeftWallInstance = -1;
		C.BackWallInstance = -1;
		C.RightWallInstance = -1;
		C.FrontWallInstance = -1;
	}
}


int32 AStation::FindOrAddCubal(int32 X, int32 Y, int32 Z)
{
	FCubalIndex Key(X, Y, Z);
	int32* CubalIndex = CubalMap.Find(Key);

	if (CubalIndex == nullptr)
	{
		int32 i = Cubal.Add(FCubal(X, Y, Z));
		CubalMap.Add(Key, i);
		return i;
	}

	return *CubalIndex;
}


void AStation::PlaceWall_Implementation(EWallDirection Direction, EWallId Id, int32 X, int32 Y, int32 Z)
{
	FTransform Transform;
	FVector Translation = FVector(X, Y, Z) * CubalSize;
	int32 LocalCubalIndex = FindOrAddCubal(X, Y, Z);
	FCubal& LocalCubal = Cubal[LocalCubalIndex];
	FWallComponent& LocalWall = FindWallComponentId(Id, EWallComponentId::Single);

	if (LocalCubal.WallInstance(Direction) >= 0)
	{
		EWallComponentId OldWallComponentId = LocalCubal.WallComponent(Direction);
		EWallId OldWallId = LocalCubal.Wall(Direction);
		FWallComponent& OldWall = FindWallComponentId(OldWallId, OldWallComponentId);

		if (OldWall.Hism != nullptr)
		{
			OldWall.Hism->RemoveInstance(LocalCubal.WallInstance(Direction));
			OldWall.Instance.RemoveSwap(LocalCubal.WallInstance(Direction));
		}
		LocalCubal.SetWallInstance(Direction, -1);
		LocalCubal.SetWall(Direction, EWallId::None);
		LocalCubal.SetWallComponent(Direction, EWallComponentId::Single);
	}

	if (LocalWall.Hism != nullptr)
	{
		switch (Direction)
		{
		case EWallDirection::Left:
			Transform = FTransform(FRotator(0, -90, 0),
			                       Translation.GridSnap(CubalSize),
			                       FVector(1.0, 1.0, 1.0));
			break;
		case EWallDirection::Right:
			Transform = FTransform(FRotator(0,  90, 0),
			                       Translation.GridSnap(CubalSize),
			                       FVector(1.0, 1.0, 1.0));
			break;
		case EWallDirection::Back:
			Transform = FTransform(FRotator(0, 180, 0),
			                       Translation.GridSnap(CubalSize),
			                       FVector(1.0, 1.0, 1.0));
			break;
		case EWallDirection::Front:
		case EWallDirection::Top:
		case EWallDirection::Bottom:
			Transform = FTransform(FRotator(0, 0, 0),
			                       Translation.GridSnap(CubalSize),
			                       FVector(1.0, 1.0, 1.0));
			break;
		}

		LocalCubal.SetWall(Direction, Id);
		LocalCubal.SetWallComponent(Direction, EWallComponentId::Single);
		LocalCubal.SetWallInstance(Direction, LocalWall.Hism->AddInstance(Transform));
		LocalWall.Instance.Add(LocalCubalIndex);
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

	FCubal& LocalCubal = Cubal[CubalIndex];

	PlaceWall(EWallDirection::Back, LocalCubal.BackWall,
	          LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Front, LocalCubal.FrontWall,
	          LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Left, LocalCubal.LeftWall,
	          LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Right, LocalCubal.RightWall,
	          LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Top, LocalCubal.Ceiling,
	          LocalCubal.Index.X, LocalCubal.Index.Y, LocalCubal.Index.Z);
	PlaceWall(EWallDirection::Bottom, LocalCubal.Floor,
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
	int32 LastXCenter;
	int32 LastYCenter;
	TArray<int8> Map;
	Map.AddZeroed(MaxX * MaxY * Z);
	auto Idx = [MaxY, Z](int x, int y, int z) { return x * MaxY * Z + y * Z + z; };

	//I don't really want to reset things here, but there is weird duplicating
	//of data with a blueprint

	ClearInstances();
	CubalMap.Empty(Cubal.Num());
	for (int i = 0; i < Cubal.Num(); ++i)
	{
		CubalMap.Add(Cubal[i].Index, i);
		PlaceCubal(i);
	}
	Random.Reset();

	for (int32 i = 0; i < Rooms; ++i)
	{
		int32 Width = Random.RandRange(3, X);
		int32 Length = Random.RandRange(3, Y);
		int32 Height = Random.RandRange(1, Z);
		int32 XOff = Random.RandRange(0, MaxX - Width - 1);
		int32 YOff = Random.RandRange(0, MaxY - Length - 1);

		/* Rooms */
		for (int32 j = XOff; j < XOff + Width; ++j)
		{
			for (int32 k = YOff; k < YOff + Length; ++k)
			{
				for (int32 l = 0; l < Height; ++l)
				{
					Map[Idx(j, k, l)] = 1;
				}
			}
		}

		/* Corridors */
		int32 XCenter = XOff + Width / 2;
		int32 YCenter = YOff + Length / 2;
		if (i != 0)
		{
			for (int32 j = LastXCenter; j <= XCenter; ++j)
			{
				if (Map[Idx(j, LastYCenter, 0)] == 0)
				{
					Map[Idx(j, LastYCenter, 0)] = 2;
				}
				if (Map[Idx(j, YCenter, 0)] == 0)
				{
					Map[Idx(j, YCenter, 0)] = 2;
				}
			}
			for (int32 j = XCenter; j < LastXCenter; ++j)
			{
				if (Map[Idx(j, LastYCenter, 0)] == 0)
				{
					Map[Idx(j, LastYCenter, 0)] = 2;
				}
				if (Map[Idx(j, YCenter, 0)] == 0)
				{
					Map[Idx(j, YCenter, 0)] = 2;
				}
			}

			for (int32 j = LastYCenter; j <= YCenter; ++j)
			{
				if (Map[Idx(LastXCenter, j, 0)] == 0)
				{
					Map[Idx(LastXCenter, j, 0)] = 2;
				}
				if (Map[Idx(XCenter, j, 0)] == 0)
				{
					Map[Idx(XCenter, j, 0)] = 2;
				}
			}
			for (int32 j = YCenter; j < LastYCenter; ++j)
			{
				if (Map[Idx(LastXCenter, j, 0)] == 0)
				{
					Map[Idx(LastXCenter, j, 0)] = 2;
				}
				if (Map[Idx(XCenter, j, 0)] == 0)
				{
					Map[Idx(XCenter, j, 0)] = 2;
				}
			}
		}
		LastXCenter = XCenter;
		LastYCenter = YCenter;
	}

	for (int32 i = 0; i < MaxX; ++i)
	{
		for (int32 j = 0; j < MaxY; ++j)
		{
			for (int32 k = 0; k < Z; ++k)
			{
				int32 RoomType = Map[Idx(i, j, k)];
				if (RoomType)
				{
					if (i == 0 || !Map[Idx(i-1,j,k)])
					{
						PlaceWall(EWallDirection::Right, EWallId::Wall, i, j, k);
						PlaceWall(EWallDirection::Left, EWallId::Wall, i-1, j, k);
					}
					else if (RoomType != Map[Idx(i-1,j,k)])
					{
						PlaceWall(EWallDirection::Right, EWallId::Doorway, i, j, k);
					}

					if (i == MaxX - 1 || !Map[Idx(i+1,j,k)])
					{
						PlaceWall(EWallDirection::Left, EWallId::Wall, i, j, k);
						PlaceWall(EWallDirection::Right, EWallId::Wall, i+1, j, k);
					}
					else if (RoomType != Map[Idx(i+1,j,k)])
					{
						PlaceWall(EWallDirection::Left, EWallId::Doorway, i, j, k);
					}

					if (j == 0 || !Map[Idx(i,j-1,k)])
					{
						PlaceWall(EWallDirection::Back, EWallId::Wall, i, j, k);
						PlaceWall(EWallDirection::Front, EWallId::Wall, i, j-1, k);
					}
					else if (RoomType != Map[Idx(i,j-1,k)])
					{
						PlaceWall(EWallDirection::Back, EWallId::Doorway, i, j, k);
					}

					if (j == MaxY - 1 || !Map[Idx(i,j+1,k)])
					{
						PlaceWall(EWallDirection::Front, EWallId::Wall, i, j, k);
						PlaceWall(EWallDirection::Back, EWallId::Wall, i, j+1, k);
					}
					else if (RoomType != Map[Idx(i,j+1,k)])
					{
						PlaceWall(EWallDirection::Front, EWallId::Doorway, i, j, k);
					}

					if (k == 0 || !Map[Idx(i,j,k-1)])
					{
						PlaceWall(EWallDirection::Bottom, EWallId::Floor, i, j, k);
						PlaceWall(EWallDirection::Top, EWallId::Ceiling, i, j, k-1);
					}

					if (k == Z - 1 || !Map[Idx(i,j,k+1)])
					{
						PlaceWall(EWallDirection::Top, EWallId::Ceiling, i, j, k);
						PlaceWall(EWallDirection::Bottom, EWallId::Floor, i, j, k+1);
					}
				}
			}
		}
	}
}


FWall& AStation::FindWallId(EWallId Id)
{
	switch(Id)
	{
	case EWallId::Wall:
		return Wall;
	case EWallId::Doorway:
		return Doorway;
	case EWallId::Window:
		return Window;
	case EWallId::Ceiling:
		return Ceiling;
	case EWallId::Floor:
		return Floor;
	default:
		return EmptyWall;
	}
}

FWallComponent& AStation::FindWallComponentId(EWallId Id, EWallComponentId CompId)
{
	FWall& Which = FindWallId(Id);

	switch(CompId)
	{
	default:
	case EWallComponentId::Single:
		return Which.Single;
	case EWallComponentId::SingleCornerOutXNeg:
		return Which.SingleCornerOutXNeg;
	case EWallComponentId::SingleCornerOutXPos:
		return Which.SingleCornerOutXPos;
	case EWallComponentId::SingleCornerOutX:
		return Which.SingleCornerOutX;
	case EWallComponentId::SingleCornerInXNeg:
		return Which.SingleCornerInXNeg;
	case EWallComponentId::SingleCornerInXPos:
		return Which.SingleCornerInXPos;
	case EWallComponentId::SingleCornerInX:
		return Which.SingleCornerInX;
	case EWallComponentId::SingleCornerInXNegOutXPos:
		return Which.SingleCornerInXNegOutXPos;
	case EWallComponentId::SingleCornerInXPosOutXNeg:
		return Which.SingleCornerInXPosOutXNeg;
	case EWallComponentId::SingleBevelX:
		return Which.SingleBevelX;
	case EWallComponentId::SingleBevelY:
		return Which.SingleBevelY;
	case EWallComponentId::SingleBevelXY:
		return Which.SingleBevelXY;
	case EWallComponentId::SingleBevelXNeg:
		return Which.SingleBevelXNeg;
	case EWallComponentId::SingleBevelXPos:
		return Which.SingleBevelXPos;
	case EWallComponentId::SingleBevelYNeg:
		return Which.SingleBevelYNeg;
	case EWallComponentId::SingleBevelYPos:
		return Which.SingleBevelYPos;
	case EWallComponentId::SingleBevelXNegY:
		return Which.SingleBevelXNegY;
	case EWallComponentId::SingleBevelXPosY:
		return Which.SingleBevelXPosY;
	case EWallComponentId::SingleBevelXYNeg:
		return Which.SingleBevelXYNeg;
	case EWallComponentId::SingleBevelXYPos:
		return Which.SingleBevelXYPos;
	case EWallComponentId::SingleBevelXNegYNeg:
		return Which.SingleBevelXNegYNeg;
	case EWallComponentId::SingleBevelXNegYPos:
		return Which.SingleBevelXNegYPos;
	case EWallComponentId::SingleBevelXPosYNeg:
		return Which.SingleBevelXPosYNeg;
	case EWallComponentId::SingleBevelXPosYPos:
		return Which.SingleBevelXPosYPos;
	case EWallComponentId::SingleBevelXNegCornerInXPos:
		return Which.SingleBevelXNegCornerInXPos;
	case EWallComponentId::SingleBevelXPosCornerInXNeg:
		return Which.SingleBevelXPosCornerInXNeg;
	case EWallComponentId::SingleBevelXNegCornerOutXPos:
		return Which.SingleBevelXNegCornerOutXPos;
	case EWallComponentId::SingleBevelXPosCornerOutXNeg:
		return Which.SingleBevelXPosCornerOutXNeg;
	}
}

/* vim: set noexpandtab: */
