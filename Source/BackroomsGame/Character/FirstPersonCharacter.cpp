// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
    // Set size for Collision Capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // Create mesh component
    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->bCastDynamicShadow = false;
    FirstPersonMesh->CastShadow = true;
    

    // Set base turn rates
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Movement Speeds
    WalkSpeed = 450.0f;
    SprintSpeed = 1200.0f;

    // HeadBob Params
    bEnableHeadbob = true;
    HeadbobFrequency = 10.0f;
    HeadbobAmplitude = 1.0f;
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bEnableHeadbob)
    {
        UpdateHeadbob(DeltaTime);
    }
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement Functions
    PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);

    // Turn and LookUp Rates
    PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpAtRate", this, &AFirstPersonCharacter::LookUpAtRate);

    // Bind Sprint Functions
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFirstPersonCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFirstPersonCharacter::StopSprint);
}

void AFirstPersonCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AFirstPersonCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AFirstPersonCharacter::TurnAtRate(float Value)
{
    AddControllerYawInput(Value);
}

void AFirstPersonCharacter::LookUpAtRate(float Value)
{
    AddControllerPitchInput(Value);
}

void AFirstPersonCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFirstPersonCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFirstPersonCharacter::UpdateHeadbob(float DeltaTime)
{
    if (!FirstPersonCameraComponent)
    {
        return;
    }

    FVector NewLocation = FirstPersonCameraComponent->GetRelativeLocation();
    NewLocation.Z += FMath::Sin(GetWorld()->TimeSeconds * HeadbobFrequency) * HeadbobAmplitude;
}