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

#include "Wall.h"

#include "Cubal.generated.h"


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
		Floor = EWallId::None;
		Ceiling = EWallId::None;
		BackWall = EWallId::None;
		FrontWall = EWallId::None;
		LeftWall = EWallId::None;
		RightWall = EWallId::None;
		FloorComponent = EWallComponentId::Single;
		CeilingComponent = EWallComponentId::Single;
		BackWallComponent = EWallComponentId::Single;
		FrontWallComponent = EWallComponentId::Single;
		LeftWallComponent = EWallComponentId::Single;
		RightWallComponent = EWallComponentId::Single;
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
		Floor = EWallId::None;
		Ceiling = EWallId::None;
		BackWall = EWallId::None;
		FrontWall = EWallId::None;
		LeftWall = EWallId::None;
		RightWall = EWallId::None;
		FloorComponent = EWallComponentId::Single;
		CeilingComponent = EWallComponentId::Single;
		BackWallComponent = EWallComponentId::Single;
		FrontWallComponent = EWallComponentId::Single;
		LeftWallComponent = EWallComponentId::Single;
		RightWallComponent = EWallComponentId::Single;
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
		case EWallDirection::Top:
			return Ceiling;
		case EWallDirection::Bottom:
			return Floor;
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
		case EWallDirection::Top:
			Ceiling = Id;
			break;
		case EWallDirection::Bottom:
			Floor = Id;
			break;
		}
	}

	// Wall Helper because static arrays are not blueprintable
	EWallComponentId WallComponent(EWallDirection Direction) const
	{
		switch (Direction)
		{
		case EWallDirection::Front:
			return FrontWallComponent;
		case EWallDirection::Back:
			return BackWallComponent;
		case EWallDirection::Right:
			return RightWallComponent;
		case EWallDirection::Left:
			return LeftWallComponent;
		case EWallDirection::Top:
			return CeilingComponent;
		case EWallDirection::Bottom:
			return FloorComponent;
		}
	}

	// Wall Helper because static arrays are not blueprintable
	void SetWallComponent(EWallDirection Direction, EWallComponentId Id)
	{
		switch (Direction)
		{
		case EWallDirection::Front:
			FrontWallComponent = Id;
			break;
		case EWallDirection::Back:
			BackWallComponent = Id;
			break;
		case EWallDirection::Right:
			RightWallComponent = Id;
			break;
		case EWallDirection::Left:
			LeftWallComponent = Id;
			break;
		case EWallDirection::Top:
			CeilingComponent = Id;
			break;
		case EWallDirection::Bottom:
			FloorComponent = Id;
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
		case EWallDirection::Top:
			return CeilingInstance;
		case EWallDirection::Bottom:
			return FloorInstance;
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
			break;
		case EWallDirection::Back:
			BackWallInstance = InstanceIndex;
			break;
		case EWallDirection::Right:
			RightWallInstance = InstanceIndex;
			break;
		case EWallDirection::Left:
			LeftWallInstance = InstanceIndex;
			break;
		case EWallDirection::Top:
			CeilingInstance = InstanceIndex;
			break;
		case EWallDirection::Bottom:
			FloorInstance = InstanceIndex;
			break;
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallComponentId FloorComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 FloorInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId Ceiling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallComponentId CeilingComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 CeilingInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId BackWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallComponentId BackWallComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 BackWallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId FrontWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallComponentId FrontWallComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 FrontWallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId RightWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallComponentId RightWallComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 RightWallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId LeftWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallComponentId LeftWallComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 LeftWallInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCubalIndex Index;
};

/* vim: set noexpandtab: */
