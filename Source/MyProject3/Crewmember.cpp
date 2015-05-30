// Fill out your copyright notice in the Description page of Project Settings.

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

    FVector  CamLoc;
    FRotator CamRot;
    Controller->GetPlayerViewPoint(CamLoc, CamRot);

    static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
    //GetWorld()->DebugDrawTraceTag   = WeaponFireTag; // Enable debug tracing
    const FVector StartTrace        = CamLoc;
    const FVector Direction         = CamRot.Vector();
    const FVector EndTrace          = StartTrace + Direction * 150;

    // Perform trace to retrieve hit info
    FCollisionQueryParams TraceParams(WeaponFireTag, true, this);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;
    
    FHitResult Hit(ForceInit);
    GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, TraceParams);

}

// Called to bind functionality to input
void ACrewmember::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    InputComponent->BindAxis("MoveForward", this, &ACrewmember::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &ACrewmember::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
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
	if ( (Controller != NULL) && (Value != 0.0f) )
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
