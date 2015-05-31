// Fill out your copyright notice in the Description page of Project Settings.

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
    UE_LOG(LogTemp, Log, TEXT("Hello"));//, *GetNameSafe(this), *GetNameSafe(Impact.GetActor()));
    Destroy();
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
