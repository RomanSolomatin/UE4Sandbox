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
