// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Item.h"
#include "Crewmember.generated.h"

UCLASS()
class MYPROJECT3_API ACrewmember : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrewmember(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FirstPersonCamera;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	void VisionTrace(FHitResult& Hit);

	UPROPERTY(Transient)
	class AItem* FocusedItem;

	void AttemptUse();

	UFUNCTION(Server, NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = Pickup)
	void ServerAttemptUse();
	virtual void ServerAttemptUse_Implementation();
	bool ServerAttemptUse_Validate();

	// Called for forwards/backward input
	void MoveForward(float Value);

	// Called for right/left input
	void MoveRight(float Value);
};

/* vim: set noexpandtab: */
