// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Engine/Texture2D.h"
#include "Components/ActorComponent.h"
#include "Item.generated.h"

UCLASS(Abstract, Blueprintable)
class MYPROJECT3_API AItem : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Test to see if item can be used
	UFUNCTION(BlueprintCallable, Category = Pickup)
	bool CanUse(ACharacter* Character, UActorComponent* Component);

	// Attempt to use item
	UFUNCTION(Server, NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = Pickup)
	void Use(ACharacter* Character, UActorComponent* Component);
	virtual void Use_Implementation(ACharacter* Character, UActorComponent* Component);
	bool Use_Validate(ACharacter* Character, UActorComponent* Component);

	// Character is looking at item
	UFUNCTION(BlueprintCosmetic, BlueprintCallable, Category = Pickup)
	bool StartFocus();

	// Character is not looking at item
	UFUNCTION(BlueprintCosmetic, BlueprintCallable, Category = Pickup)
	bool EndFocus();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Pickup)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Pickup)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Pickup)
	bool bShowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Pickup)
	bool bIsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Pickup)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Pickup)
	UTexture2D* InventoryImage;
};
