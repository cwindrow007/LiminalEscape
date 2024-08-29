// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "BackroomsGame/Menus/GameSettingsManager.h"
#include "EntitySystem/MovieSceneEntitySystemTypes.h"

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
    //GameSettingsManager = CreateDefaultSubobject<UGameSettingsManager>(TEXT("GameSettingsManager"));

    //Set up Input Actions from GameSettings Manager
    //IA_Jump = GameSettingsManager->GetJumpAction();
    //IA_Sprint = GameSettingsManager->GetSprintAction();
    //IA_MoveForward = GameSettingsManager->GetMoveForwardAction();
    //IA_MoveRight = GameSettingsManager->GetMoveRightAction();
    //IA_Turn = GameSettingsManager->GetTurnAction(); // Assuming there's a Turn action
    //IA_LookUp = GameSettingsManager->GetLookUpAction();

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
    if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
       if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
       {
           Subsystem->AddMappingContext(HazzyMappingContext, 0);
       }
    }
}

//Hazzy Move Forward
void AFirstPersonCharacter::HazzyMove(const FInputActionValue& Value)
{
    const FVector2D HazzyMovementVector = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, HazzyMovementVector.Y);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, HazzyMovementVector.X);
    
}

//Hazzy Look Action
void AFirstPersonCharacter::HazzyLook(const FInputActionValue& Value)
{
    const  FVector2D LookAxisValue = Value.Get<FVector2D>();
    if(GetController())
    {
        AddControllerYawInput(LookAxisValue.X);
        AddControllerPitchInput(LookAxisValue.Y);
    }
}
void AFirstPersonCharacter::HazzyJump()
{
    Jump();
}



// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

 

    //if (bEnableHeadbob)
    //{
       // UpdateHeadbob(DeltaTime);
    //}
}

//Called Bind Functionality to input

void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(HazzyMoveAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyMove);
        EnhancedInputComponent->BindAction(HazzyLookAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyLook);
        EnhancedInputComponent->BindAction(HazzyJumpAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyJump);
        //EnhancedInputComponent->BindAction(HazzyInteractAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyInteract);
        //EnhancedInputComponent->BindAction(HazzySprintAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyJump);
        EnhancedInputComponent->BindAction(HazzySprintAction, ETriggerEvent::Started, this, &AFirstPersonCharacter::StartSprint);
        EnhancedInputComponent->BindAction(HazzySprintAction, ETriggerEvent::Completed, this, &AFirstPersonCharacter::StopSprint);
        
      
    }
}


void AFirstPersonCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFirstPersonCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

 /*void AFirstPersonCharacter::UpdateHeadbob(float DeltaTime)
{
    if (!FirstPersonCameraComponent)
    {
        return;
    }

    FVector NewLocation = FirstPersonCameraComponent->GetRelativeLocation();
    NewLocation.Z += FMath::Sin(GetWorld()->TimeSeconds * HeadbobFrequency) * HeadbobAmplitude;
}
*/