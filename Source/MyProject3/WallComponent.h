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

#include "WallComponent.generated.h"


class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;

UENUM(BlueprintType)
enum class EWallComponentId : uint8
{
	Single                       = 0x00 UMETA(DisplayName="Single"),
	SingleCornerInXNeg           = 0x01 UMETA(DisplayName="SingleCornerInXNeg"),
	SingleCornerOutXNeg          = 0x02 UMETA(DisplayName="SingleCornerOutXNeg"),
	SingleBevelXNeg              = 0x03 UMETA(DisplayName="SingleBevelXNeg"),

	SingleCornerInXPos           = 0x04 UMETA(DisplayName="SingleCornerInXPos"),
	SingleCornerInX              = 0x05 UMETA(DisplayName="SingleCornerInX"),
	SingleCornerInXPosOutXNeg    = 0x06 UMETA(DisplayName="SingleCornerInXPosOutXNeg"),
	SingleBevelXNegCornerInXPos  = 0x07 UMETA(DisplayName="SingleBevelXNegCornerInXPos"),

	SingleCornerOutXPos          = 0x08 UMETA(DisplayName="SingleCornerOutXPos"),
	SingleCornerInXNegOutXPos    = 0x09 UMETA(DisplayName="SingleCornerInXNegOutXPos"),
	SingleCornerOutX             = 0x0a UMETA(DisplayName="SingleCornerOutX"),
	SingleBevelXNegCornerOutXPos = 0x0b UMETA(DisplayName="SingleBevelXNegCornerOutXPos"),

	SingleBevelXPos              = 0x0c UMETA(DisplayName="SingleBevelXPos"),
	SingleBevelXPosCornerInXNeg  = 0x0d UMETA(DisplayName="SingleBevelXPosCornerInXNeg"),
	SingleBevelXPosCornerOutXNeg = 0x0e UMETA(DisplayName="SingleBevelXPosCornerOutXNeg"),
	SingleBevelX                 = 0x0f UMETA(DisplayName="SingleBevelX"),

	// Only for floors and ceilings
	SingleBevelYNeg              = 0x10 UMETA(DisplayName="SingleBevelYNeg"),
	SingleBevelXNegYNeg          = 0x11 UMETA(DisplayName="SingleBevelXNegYNeg"),
	SingleBevelXPosYNeg          = 0x12 UMETA(DisplayName="SingleBevelXPosYNeg"),
	SingleBevelXYNeg             = 0x13 UMETA(DisplayName="SingleBevelXYNeg"),
	SingleBevelYPos              = 0x14 UMETA(DisplayName="SingleBevelYNeg"),
	SingleBevelXNegYPos          = 0x15 UMETA(DisplayName="SingleBevelXNegYPos"),
	SingleBevelXPosYPos          = 0x16 UMETA(DisplayName="SingleBevelXPosYPos"),
	SingleBevelXYPos             = 0x17 UMETA(DisplayName="SingleBevelXYPos"),
	SingleBevelY                 = 0x18 UMETA(DisplayName="SingleBevelY"),
	SingleBevelXNegY             = 0x19 UMETA(DisplayName="SingleBevelXNegY"),
	SingleBevelXPosY             = 0x1a UMETA(DisplayName="SingleBevelXPosY"),
	SingleBevelXY                = 0x1b UMETA(DisplayName="SingleBevelXY"),

	Max UMETA(Hidden)
};

USTRUCT(BluePrintable)
struct FWallComponent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintReadOnly)
	EWallComponentId Id;

	UPROPERTY(BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent* Hism;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Instance;
};

/* vim: set noexpandtab: */
