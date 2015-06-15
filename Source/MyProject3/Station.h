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

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Math/RandomStream.h"

#include "Station.generated.h"


class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;
class USceneComponent;


UENUM(BlueprintType)
enum class EWallDirection : uint8
{
	Front UMETA(DisplayName="Front"),
	Back UMETA(DisplayName="Back"),
	Right UMETA(DisplayName="Right"),
	Left UMETA(DisplayName="Left"),
};

UENUM(BlueprintType)
enum class EFloorId : uint8
{
	None UMETA(DisplayName="None"),
	Floor UMETA(DisplayName="Floor"),
};

UENUM(BlueprintType)
enum class ECeilingId : uint8
{
	None UMETA(DisplayName="None"),
	Ceiling UMETA(DisplayName="Ceiling"),
};

UENUM(BlueprintType)
enum class EWallId : uint8
{
	None UMETA(DisplayName="None"),
	Wall UMETA(DisplayName="Wall"),
	Doorway UMETA(DisplayName="Doorway"),
	Window UMETA(DisplayName="Window"),
};

USTRUCT(BluePrintable)
struct FCubalIndex
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Z;

	FCubalIndex()
	{
		X = 0;
		Y = 0;
		Z = 0;
	}

	FCubalIndex(int32 XPos, int32 YPos, int32 ZPos)
	{
		X = XPos;
		Y = YPos;
		Z = ZPos;
	}

	bool operator<(const FCubalIndex& Other) const
	{
		if (Z < Other.Z) return true;
		if (Z > Other.Z) return false;
		if (Y < Other.Y) return true;
		if (Y > Other.Y) return false;
		if (X < Other.X) return true;
		return false;
	}

	bool operator<=(const FCubalIndex& Other) const
	{
		if (Z < Other.Z) return true;
		if (Z > Other.Z) return false;
		if (Y < Other.Y) return true;
		if (Y > Other.Y) return false;
		if (X <= Other.X) return true;
		return false;
	}

	bool operator>(const FCubalIndex& Other) const
	{
		if (Z > Other.Z) return true;
		if (Z < Other.Z) return false;
		if (Y > Other.Y) return true;
		if (Y < Other.Y) return false;
		if (X > Other.X) return true;
		return false;
	}

	bool operator>=(const FCubalIndex& Other) const
	{
		if (Z > Other.Z) return true;
		if (Z < Other.Z) return false;
		if (Y > Other.Y) return true;
		if (Y < Other.Y) return false;
		if (X >= Other.X) return true;
		return false;
	}

	bool operator==(const FCubalIndex& Other) const
	{
		return (Z == Other.Z && Y == Other.Y && X == Other.X);
	}

	bool operator!=(const FCubalIndex& Other) const
	{
		return (Z != Other.Z || Y != Other.Y || X != Other.X);
	}

};
uint32 GetTypeHash(const FCubalIndex& A);


USTRUCT(BluePrintable)
struct FCubal
{
	GENERATED_USTRUCT_BODY()

	FCubal()
	{
		FloorInstance = -1;
		CeilingInstance = -1;
		BackWallInstance = -1;
		FrontWallInstance = -1;
		LeftWallInstance = -1;
		RightWallInstance = -1;
		Floor = EFloorId::None;
		Ceiling = ECeilingId::None;
		BackWall = EWallId::None;
		FrontWall = EWallId::None;
		LeftWall = EWallId::None;
		RightWall = EWallId::None;
	}

	FCubal(int32 X, int32 Y, int32 Z)
		: Index(X, Y, Z)
	{
		FloorInstance = -1;
		CeilingInstance = -1;
		BackWallInstance = -1;
		FrontWallInstance = -1;
		LeftWallInstance = -1;
		RightWallInstance = -1;
		Floor = EFloorId::None;
		Ceiling = ECeilingId::None;
		BackWall = EWallId::None;
		FrontWall = EWallId::None;
		LeftWall = EWallId::None;
		RightWall = EWallId::None;
	}

	void SetPosition(int32 X, int32 Y, int32 Z)
	{
		Index.X = X;
		Index.Y = Y;
		Index.Z = Z;
	}

	// Wall Helper because static arrays are not blueprintable
	EWallId Wall(EWallDirection Direction) const
	{
		switch (Direction)
		{
		case EWallDirection::Front:
			return FrontWall;
		case EWallDirection::Back:
			return BackWall;
		case EWallDirection::Right:
			return RightWall;
		case EWallDirection::Left:
			return LeftWall;
		}
	}

	// Wall Helper because static arrays are not blueprintable
	void SetWall(EWallDirection Direction, EWallId Id)
	{
		switch (Direction)
		{
		case EWallDirection::Front:
			FrontWall = Id;
			break;
		case EWallDirection::Back:
			BackWall = Id;
			break;
		case EWallDirection::Right:
			RightWall = Id;
			break;
		case EWallDirection::Left:
			LeftWall = Id;
			break;
		}
	}

	// Wall Helper because static arrays are not blueprintable
	int32 WallInstance(EWallDirection Direction) const
	{
		switch (Direction)
		{
		case EWallDirection::Front:
			return FrontWallInstance;
		case EWallDirection::Back:
			return BackWallInstance;
		case EWallDirection::Right:
			return RightWallInstance;
		case EWallDirection::Left:
			return LeftWallInstance;
		}
		return -1;
	}

	// Wall Helper because static arrays are not blueprintable
	void SetWallInstance(EWallDirection Direction, int32 InstanceIndex)
	{
		switch (Direction)
		{
		case EWallDirection::Front:
			FrontWallInstance = InstanceIndex;
		case EWallDirection::Back:
			BackWallInstance = InstanceIndex;
		case EWallDirection::Right:
			RightWallInstance = InstanceIndex;
		case EWallDirection::Left:
			LeftWallInstance = InstanceIndex;
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFloorId Floor;

	UPROPERTY(BlueprintReadOnly)
	int32 FloorInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECeilingId Ceiling;

	UPROPERTY(BlueprintReadOnly)
	int32 CeilingInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId BackWall;

	UPROPERTY(BlueprintReadOnly)
	int32 BackWallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId FrontWall;

	UPROPERTY(BlueprintReadOnly)
	int32 FrontWallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId RightWall;

	UPROPERTY(BlueprintReadOnly)
	int32 RightWallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId LeftWall;

	UPROPERTY(BlueprintReadOnly)
	int32 LeftWallInstance;

	UPROPERTY(BlueprintReadWrite)
	FCubalIndex Index;
};


UCLASS(Abstract, Blueprintable)
class MYPROJECT3_API AStation : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStation();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category=Generate)
	int32 FindOrAddCubal(int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceFloor(EFloorId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceCeiling(ECeilingId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceWall(EWallDirection dir, EWallId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Category=Generate)
	void PlaceCubal(int32 CubalIndex);

	UFUNCTION(BlueprintCallable, Category=Drawing)
	void ClearInstances();

	UFUNCTION(BlueprintCallable, Category=Generate)
	void GenerateRandomMap(int32 X, int32 Y, int32 Z, int32 Rooms);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCubal> Cubal;
	TMap<FCubalIndex, int32> CubalMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Size)
	FVector CubalSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMesh* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMesh* CeilingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMesh* DoorwayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMesh* WindowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *FloorComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> FloorInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *CeilingComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> CeilingInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *WallComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> WallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *DoorwayComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> DoorwayInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *WindowComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> WindowInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRandomStream Random;

};

/* vim: set noexpandtab: */
