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
	Single UMETA(DisplayName="Single"),

	// Corner Types
	SingleCornerOutXNeg UMETA(DisplayName="SingleCornerOutXNeg"),
	SingleCornerOutXPos UMETA(DisplayName="SingleCornerOutXPos"),
	SingleCornerOutX UMETA(DisplayName="SingleCornerOutX"),
	SingleCornerInXNeg UMETA(DisplayName="SingleCornerInXNeg"),
	SingleCornerInXPos UMETA(DisplayName="SingleCornerInXPos"),
	SingleCornerInX UMETA(DisplayName="SingleCornerInX"),
	SingleCornerInXNegOutXPos UMETA(DisplayName="SingleCornerInXNegOutXPos"),
	SingleCornerInXPosOutXNeg UMETA(DisplayName="SingleCornerInXPosOutXNeg"),

	// Bevel Types
	SingleBevelX UMETA(DisplayName="SingleBevelX"),
	SingleBevelY UMETA(DisplayName="SingleBevelY"),
	SingleBevelXY UMETA(DisplayName="SingleBevelXY"),
	SingleBevelXNeg UMETA(DisplayName="SingleBevelXNeg"),
	SingleBevelXPos UMETA(DisplayName="SingleBevelXPos"),
	SingleBevelYNeg UMETA(DisplayName="SingleBevelYNeg"),
	SingleBevelYPos UMETA(DisplayName="SingleBevelYPos"),
	SingleBevelXNegY UMETA(DisplayName="SingleBevelXNegY"),
	SingleBevelXPosY UMETA(DisplayName="SingleBevelXPosY"),
	SingleBevelXYNeg UMETA(DisplayName="SingleBevelXYNeg"),
	SingleBevelXYPos UMETA(DisplayName="SingleBevelXYPos"),
	SingleBevelXNegYNeg UMETA(DisplayName="SingleBevelXNegYNeg"),
	SingleBevelXNegYPos UMETA(DisplayName="SingleBevelXNegYPos"),
	SingleBevelXPosYNeg UMETA(DisplayName="SingleBevelXPosYNeg"),
	SingleBevelXPosYPos UMETA(DisplayName="SingleBevelXPosYPos"),

	// Mixed Corner-Bevel Types
	SingleBevelXNegCornerInXPos UMETA(DisplayName="SingleBevelXNegCornerInXPos"),
	SingleBevelXPosCornerInXNeg UMETA(DisplayName="SingleBevelXPosCornerInXNeg"),
	SingleBevelXNegCornerOutXPos UMETA(DisplayName="SingleBevelXNegCornerOutXPos"),
	SingleBevelXPosCornerOutXNeg UMETA(DisplayName="SingleBevelXPosCornerOutXNeg"),

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
