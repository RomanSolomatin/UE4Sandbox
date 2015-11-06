/*
 * Copyright (c) 2015 Kevin Nygaard
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
#include "Item.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItem::AItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->AttachTo(RootComponent);
};

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, Name);
	DOREPLIFETIME(AItem, bShowName);
	DOREPLIFETIME(AItem, bIsStackable);
	DOREPLIFETIME(AItem, MaxStackSize);
	DOREPLIFETIME(AItem, InventoryImage);
}


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Test to see if item can be used
bool AItem::CanUse(ACharacter* Character, UActorComponent* Component)
{
	return true;
}

// Attempt to use item
void AItem::Use_Implementation(ACharacter* Character, UActorComponent* Component)
{
	if (StaticMeshComponent) {
		StaticMeshComponent->DestroyComponent();
	}
}

// Validate inputs
bool AItem::Use_Validate(ACharacter* Character, UActorComponent* Component)
{
	return true;
}

// Character is looking at item
bool AItem::StartFocus()
{
	return false;
}

// Character is not looking at item
bool AItem::EndFocus()
{
	return false;
}

/* vim: set noexpandtab: */
