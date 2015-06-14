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
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Station.generated.h"


class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;
class USceneComponent;


UENUM(BlueprintType)
enum class ETileId : uint8

{
	None UMETA(DisplayName="None"),
	Floor UMETA(DisplayName="Floor"),
	Wall UMETA(DisplayName="Wall"),
	Ceiling UMETA(DisplayName="Ceiling"),
	Doorway UMETA(DisplayName="Doorway"),
	Window UMETA(DisplayName="Window"),
};

USTRUCT(BluePrintable)
struct FRoom
{
	GENERATED_USTRUCT_BODY()

	FRoom() {}
	
	void Setup(int32 XPos, int32 YPos, int32 ZLevel)
	{
		Floor = ETileId::None;
		Ceiling = ETileId::None;
		LeftWall = ETileId::None;
		FrontWall = ETileId::None;
		RightWall = ETileId::None;
		BackWall = ETileId::None;
		X = XPos;
		Y = YPos;
		Z = ZLevel;
	}

	UPROPERTY(BlueprintReadOnly)
	ETileId Floor;

	UPROPERTY(BlueprintReadOnly)
	ETileId Ceiling;

	UPROPERTY(BlueprintReadOnly)
	ETileId LeftWall;

	UPROPERTY(BlueprintReadOnly)
	ETileId FrontWall;

	UPROPERTY(BlueprintReadOnly)
	ETileId RightWall;

	UPROPERTY(BlueprintReadOnly)
	ETileId BackWall;

	UPROPERTY(BlueprintReadOnly)
	int32 X;

	UPROPERTY(BlueprintReadOnly)
	int32 Y;

	UPROPERTY(BlueprintReadOnly)
	int32 Z;
};

USTRUCT(BluePrintable)
struct FRoomRow
{
	GENERATED_USTRUCT_BODY()
	    
	FRoomRow() {}

	void AddNewRoom(int X)
	{
		Room.Insert(FRoom(), X);
	}

	void Setup(int32 YPos, int32 ZLevel, int32 RoomCount)
	{
		Z = ZLevel;
		Y = YPos;
		Room.Reserve(RoomCount);
		for (int32 i = 0; i < RoomCount; ++i)
		{
			AddNewRoom(i);
			Room[i].Setup(i, Y, Z);
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoom> Room;

	UPROPERTY(BlueprintReadOnly)
	int32 Y;

	UPROPERTY(BlueprintReadOnly)
	int32 Z;
};

USTRUCT(BluePrintable)
struct FDeck
{
	GENERATED_USTRUCT_BODY();

	FDeck() {}

	void AddNewRow(int Y)
	{
	    Row.Insert(FRoomRow(), Y);
	}

	void Setup(int32 ZLevel, int32 RowCount, int32 ColumnCount)
	{
		Z = ZLevel;
		Row.Reserve(RowCount);
		for (int32 i = 0; i < RowCount; ++i) {
			AddNewRow(i);
			Row[i].Setup(i, Z, ColumnCount);
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoomRow> Row;

	UPROPERTY(BlueprintReadOnly)
	int32 Z;
};

USTRUCT(BluePrintable)
struct FStationMap
{
	GENERATED_USTRUCT_BODY();

	FStationMap() {}

	void AddNewDeck(int Z)
	{
	    Deck.Insert(FDeck(), Z);
	}

	void Setup(int32 DeckCount, int32 RowCount, int32 ColumnCount)
	{
		Deck.Reserve(DeckCount);
		for (int32 i = 0; i < DeckCount; ++i) {
			AddNewDeck(i);
			Deck[i].Setup(i, RowCount, ColumnCount);
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDeck> Deck;
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

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceFloor(ETileId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceCeiling(ETileId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceLeftWall(ETileId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceFrontWall(ETileId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceRightWall(ETileId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceBackWall(ETileId type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Category=Generate)
	void GenerateRandomMap(int32 X, int32 Y, int32 Z);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStationMap Map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Size)
	FVector TileSize;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *CeilingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *WallComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *DoorwayComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *WindowComponent;
};

/* vim: set noexpandtab: */
