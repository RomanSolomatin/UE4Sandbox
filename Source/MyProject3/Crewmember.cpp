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
#include "Crewmember.h"

// Sets default values
ACrewmember::ACrewmember(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = true;
	bUseControllerRotationRoll  = false;

	/*
	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("PawnMesh1P"));
	Mesh1P->AttachParent = GetCapsuleComponent();
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh1P->bChartDistanceFactor = false;
	Mesh1P->SetCollisionObjectType(ECC_Pawn);
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);
	*/

	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;
	//GetCharacterMovement()->AirControl = 0.2f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCamera->bUsePawnControlRotation = true;
}

/*
void ACrewmember::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ACrewmember, FocusedItem);
}
*/

// Called when the game starts or when spawned
void ACrewmember::BeginPlay()
{
	Super::BeginPlay();
	//FirstPersonCamera->AttachTo(GetMesh(), "headSocket");

}

// Called every frame
void ACrewmember::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Hit(ForceInit);
	VisionTrace(Hit);

	auto Item	= Cast<AItem>(Hit.GetActor());
	FocusedItem = Item;
}

void ACrewmember::VisionTrace(FHitResult& Hit)
{
	FVector  CamLoc;
	FRotator CamRot;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);

	static FName CharacterVisionTrace = FName(TEXT("CharacterVisionTrace"));
	//GetWorld()->DebugDrawTraceTag     = CharacterVisionTrace; // Enable debug tracing
	const FVector StartTrace          = CamLoc;
	const FVector Direction           = CamRot.Vector();
	const FVector EndTrace            = StartTrace + Direction * 180;

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(CharacterVisionTrace, true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, TraceParams);
	//UE_LOG(LogTemp, Log, TEXT("Looking at %s"), *GetNameSafe(Hit.GetActor()));
}

// Called to bind functionality to input
void ACrewmember::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Use", IE_Pressed, this, &ACrewmember::AttemptUse);
	InputComponent->BindAxis("MoveForward", this, &ACrewmember::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACrewmember::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ACrewmember::AttemptUse()
{
	UE_LOG(LogTemp, Log, TEXT("Client wants to use %s"), *GetNameSafe(FocusedItem));
	if (FocusedItem && FocusedItem->CanUse(this, NULL))
	{
		UE_LOG(LogTemp, Log, TEXT("Asking server to use"));
		ServerAttemptUse();
	}
}

void ACrewmember::ServerAttemptUse_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Server validating use"));

	// Have server cast ray to see what object they are looking at, since we
	// don't trust the client
	FHitResult Hit(ForceInit);
	VisionTrace(Hit);
	auto Item = Cast<AItem>(Hit.GetActor());

	if (Item && Item->CanUse(this, NULL))
	{
		UE_LOG(LogTemp, Log, TEXT("Use validated. Using item."));
		Item->Use(this, NULL);
	}
}

bool ACrewmember::ServerAttemptUse_Validate()
{
	return true;
}

void ACrewmember::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACrewmember::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

	}
}

/* vim: set noexpandtab: */
