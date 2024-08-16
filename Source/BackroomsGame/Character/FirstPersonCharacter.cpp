// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "BackroomsGame/Menus/GameSettingsManager.h"

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
    
    //Initialize Game Settings Manager
    GameSettingsManager = CreateDefaultSubobject<UGameSettingsManager>(TEXT("GameSettingsManager"));

    //Set up Input Actions from GameSettings Manager
    IA_Jump = GameSettingsManager->GetJumpAction();
    IA_Sprint = GameSettingsManager->GetSprintAction();
    IA_MoveForward = GameSettingsManager->GetMoveForwardAction();
    IA_MoveRight = GameSettingsManager->GetMoveRightAction();
    IA_Turn = GameSettingsManager->GetTurnAction(); // Assuming there's a Turn action
    IA_LookUp = GameSettingsManager->GetLookUpAction();

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

    if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::Jump);
        EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AFirstPersonCharacter::StopJumping);

        EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AFirstPersonCharacter::StartSprint);
        EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AFirstPersonCharacter::StopSprint);

        EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::MoveForwardEnhanced);
        EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::MoveRightEnhanced);
        EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::TurnEnhanced);
        EnhancedInputComponent->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::LookUpEnhanced);
        
    }
}

void AFirstPersonCharacter::MoveForwardEnhanced(const FInputActionValue& Value)
{
    if (Controller && Value.Get<float>() != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value.Get<float>());
    }
}

void AFirstPersonCharacter::MoveRightEnhanced(const FInputActionValue& Value)
{
    if(Controller && Value.Get<float>()!= 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0,Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value.Get<float>());
    }
}

void AFirstPersonCharacter::TurnEnhanced(const FInputActionValue& Value)
{
    AddControllerYawInput(Value.Get<float>());
}

void AFirstPersonCharacter::LookUpEnhanced(const FInputActionValue& Value)
{
    AddControllerPitchInput(Value.Get<float>());
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
