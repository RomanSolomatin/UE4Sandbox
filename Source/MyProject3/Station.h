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

#include "Cubal.h"
#include "Wall.h"

#include "Station.generated.h"


class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;
class USceneComponent;


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
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category=Generate)
	int32 FindOrAddCubal(int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Server, NetMulticast, Reliable, WithValidation, Category=Generate)
	void PlaceWall(EWallDirection Dir, EWallId Type, int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable, Category=Generate)
	void PlaceCubal(int32 CubalIndex);

	UFUNCTION(BlueprintCallable, Category=Drawing)
	void ClearInstances();

	UFUNCTION(BlueprintCallable, Category=Generate)
	void GenerateRandomMap(int32 X, int32 Y, int32 Z, int32 Rooms);

protected:
	UFUNCTION(BlueprintCallable, Category=Generate)
	FWall& FindWallId(EWallId Id);

	UFUNCTION(BlueprintCallable, Category=Generate)
	FWallComponent& FindWallComponentId(EWallId Id, EWallComponentId CompId);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCubal> Cubal;
	TMap<FCubalIndex, int32> CubalMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRandomStream Random;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Size)
	float CubalSize;

	UPROPERTY(BlueprintReadOnly)
	FWall EmptyWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWall Wall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWall Doorway;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWall Window;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWall Ceiling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWall Floor;
};

/* vim: set noexpandtab: */
