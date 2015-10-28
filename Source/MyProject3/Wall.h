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

#include "WallComponent.h"

#include "Wall.generated.h"


UENUM(BlueprintType)
enum class EWallDirection : uint8
{
	Front UMETA(DisplayName="Front"),
	Left UMETA(DisplayName="Left"),
	Back UMETA(DisplayName="Back"),
	Right UMETA(DisplayName="Right"),
	Top UMETA(DisplayName="Top"),
	Bottom UMETA(DisplayName="Bottom"),
};

UENUM(BlueprintType)
enum class EWallId : uint8
{
	None UMETA(DisplayName="None"),

	Wall UMETA(DisplayName="Wall"),
	Doorway UMETA(DisplayName="Doorway"),
	Window UMETA(DisplayName="Window"),
	Ceiling UMETA(DisplayName="Ceiling"),
	Floor UMETA(DisplayName="Floor"),

	Max UMETA(Hidden)
};

USTRUCT(BluePrintable)
struct FWall
{
	GENERATED_USTRUCT_BODY()

	FWall()
	{
		Single.Id = EWallComponentId::Single;
		SingleCornerOutXNeg.Id = EWallComponentId::SingleCornerOutXNeg;
		SingleCornerOutXPos.Id = EWallComponentId::SingleCornerOutXPos;
		SingleCornerOutX.Id = EWallComponentId::SingleCornerOutX;
		SingleCornerInXNeg.Id = EWallComponentId::SingleCornerInXNeg;
		SingleCornerInXPos.Id = EWallComponentId::SingleCornerInXPos;
		SingleCornerInX.Id = EWallComponentId::SingleCornerInX;
		SingleCornerInXNegOutXPos.Id = EWallComponentId::SingleCornerInXNegOutXPos;
		SingleCornerInXPosOutXNeg.Id = EWallComponentId::SingleCornerInXPosOutXNeg;
		SingleBevelX.Id = EWallComponentId::SingleBevelX;
		SingleBevelY.Id = EWallComponentId::SingleBevelY;
		SingleBevelXY.Id = EWallComponentId::SingleBevelXY;
		SingleBevelXNeg.Id = EWallComponentId::SingleBevelXNeg;
		SingleBevelXPos.Id = EWallComponentId::SingleBevelXPos;
		SingleBevelYNeg.Id = EWallComponentId::SingleBevelYNeg;
		SingleBevelYPos.Id = EWallComponentId::SingleBevelYPos;
		SingleBevelXNegY.Id = EWallComponentId::SingleBevelXNegY;
		SingleBevelXPosY.Id = EWallComponentId::SingleBevelXPosY;
		SingleBevelXYNeg.Id = EWallComponentId::SingleBevelXYNeg;
		SingleBevelXYPos.Id = EWallComponentId::SingleBevelXYPos;
		SingleBevelXNegYNeg.Id = EWallComponentId::SingleBevelXNegYNeg;
		SingleBevelXNegYPos.Id = EWallComponentId::SingleBevelXNegYPos;
		SingleBevelXPosYNeg.Id = EWallComponentId::SingleBevelXPosYNeg;
		SingleBevelXPosYPos.Id = EWallComponentId::SingleBevelXPosYPos;
		SingleBevelXNegCornerInXPos.Id = EWallComponentId::SingleBevelXNegCornerInXPos;
		SingleBevelXPosCornerInXNeg.Id = EWallComponentId::SingleBevelXPosCornerInXNeg;
		SingleBevelXNegCornerOutXPos.Id = EWallComponentId::SingleBevelXNegCornerOutXPos;
		SingleBevelXPosCornerOutXNeg.Id = EWallComponentId::SingleBevelXPosCornerOutXNeg;
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWallId Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent Single;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerOutXNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerOutXPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerOutX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerInXNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerInXPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerInX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerInXNegOutXPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleCornerInXPosOutXNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelYNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelYPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXNegY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXPosY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXYNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXYPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXNegYNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXNegYPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXPosYNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXPosYPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXNegCornerInXPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXPosCornerInXNeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXNegCornerOutXPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWallComponent SingleBevelXPosCornerOutXNeg;
};


static inline EWallDirection WallLeftDirection(EWallDirection From)
{
	switch (From)
	{
	default:
	case EWallDirection::Front: return EWallDirection::Left;
	case EWallDirection::Left:  return EWallDirection::Back;
	case EWallDirection::Back:  return EWallDirection::Right;
	case EWallDirection::Right: return EWallDirection::Front;
	}
}


static inline EWallDirection WallRightDirection(EWallDirection From)
{
	switch (From)
	{
	default:
	case EWallDirection::Front: return EWallDirection::Right;
	case EWallDirection::Right: return EWallDirection::Back;
	case EWallDirection::Back:  return EWallDirection::Left;
	case EWallDirection::Left:  return EWallDirection::Front;
	}
}


static inline int WallOffsetX(EWallDirection From, int Pos, int XOff, int YOff)
{
	switch (From)
	{
	default:
	case EWallDirection::Front: return Pos + XOff;
	case EWallDirection::Right: return Pos + YOff;
	case EWallDirection::Back:  return Pos - XOff;
	case EWallDirection::Left:  return Pos - YOff;
	case EWallDirection::Top:   return Pos + XOff;
	case EWallDirection::Bottom:return Pos + XOff;
	}
}


static inline int WallOffsetY(EWallDirection From, int Pos, int XOff, int YOff)
{
	switch (From)
	{
	default:
	case EWallDirection::Front: return Pos + YOff;
	case EWallDirection::Right: return Pos + XOff;
	case EWallDirection::Back:  return Pos - YOff;
	case EWallDirection::Left:  return Pos - XOff;
	case EWallDirection::Top:   return Pos - YOff;
	case EWallDirection::Bottom:return Pos + YOff;
	}
}

/* vim: set noexpandtab: */
