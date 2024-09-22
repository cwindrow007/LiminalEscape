// Fill out your copyright notice in the Description page of Project Settings.

//Dependencies
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
#include "DrawDebugHelpers.h"
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

    // Movement Speeds
    WalkSpeed = 450.0f;
    SprintSpeed = 1200.0f;

    // HeadBob Params
    bEnableHeadbob = true;
    HeadbobFrequency = 10.0f;
    HeadbobAmplitude = 1.0f;

    //Interaction Time
    InteractionCheckFrequency = 0.1;
    InteractionCheckDistance = 235.0f;
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
    Super::BeginPlay();

    //HudCast
    HUD = Cast<AHazzyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    
    if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
       if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
       {
           Subsystem->AddMappingContext(HazzyMappingContext, 0);
       }
    }

    //Sanity System Set Variable
    
    SanitySystem = NewObject<USanitySystem>();
    SanitySystem->Initialize(100.0f, 0.238f);
    SanitySystem->StartSanityDepletion(GetWorld());
}

//==================================================
//MOVEMENT FUNCTIONS
//==================================================
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(HazzyMoveAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyMove);
        EnhancedInputComponent->BindAction(HazzyLookAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyLook);
        EnhancedInputComponent->BindAction(HazzyJumpAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::HazzyJump);
        EnhancedInputComponent->BindAction(HazzyInteractAction, ETriggerEvent::Started, this, &AFirstPersonCharacter::BeginInteract);
        EnhancedInputComponent->BindAction(HazzyInteractAction, ETriggerEvent::Completed, this, &AFirstPersonCharacter::EndInteract);
        EnhancedInputComponent->BindAction(HazzySprintAction, ETriggerEvent::Started, this, &AFirstPersonCharacter::StartSprint);
        EnhancedInputComponent->BindAction(HazzySprintAction, ETriggerEvent::Completed, this, &AFirstPersonCharacter::StopSprint);
        
      
    }
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }

 

    //if (bEnableHeadbob)
    //{
    // UpdateHeadbob(DeltaTime);
    //}
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

//Sprint Handling
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
//==================================================
//INTERACTION FUNCTIONS
//==================================================

void AFirstPersonCharacter::PerformInteractionCheck()
{
    InteractionData.LastInteractionCheckTime = GetWorld()-> GetTimeSeconds();

    FVector TraceStart{GetPawnViewLocation()};
    FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

    float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

    if(LookDirection > 0)
    {
        //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

        //Line Tracing
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        FHitResult TraceHit;

        if(GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd,ECC_Visibility,QueryParams))
        {
            if(TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
            {
                if(TraceHit.GetActor() != InteractionData.CurrentInteractable )
                {
                    FoundInteractable(TraceHit.GetActor());
                    return;
                }

                if(TraceHit.GetActor() == InteractionData.CurrentInteractable)
                {
                    return;
                }
            }
        }
    }
    NointeractableFound();
}

//Found interaction for Interactable Items
void AFirstPersonCharacter::FoundInteractable(AActor* NewInteractable)
{
    if(IsInteracting())
    {
        EndInteract();
    }

    if(InteractionData.CurrentInteractable)
    {
        TargetInteractable =  InteractionData.CurrentInteractable;
        TargetInteractable->EndFocus();
    }

    InteractionData.CurrentInteractable = NewInteractable;
    TargetInteractable = NewInteractable;

    HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

    TargetInteractable->BeginFocus();
}

void AFirstPersonCharacter::NointeractableFound()
{
    if(IsInteracting())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    }

    if(InteractionData.CurrentInteractable)
    {
        if(IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->EndFocus();
        }

        HUD->HideInteractionWidget();
        
        InteractionData.CurrentInteractable = nullptr;
        TargetInteractable = nullptr;
    
    }
}

//Begin Interaction Function
void AFirstPersonCharacter::BeginInteract()
{
    //Verification to ensure nothing has changed since beginning interaction state
    PerformInteractionCheck();
//If Interaction is the current interaction then set 0 duration to interact set a timer for the duration and do not loop
    if(InteractionData.CurrentInteractable)
    {
        if(IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->BeginInteract();

            if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
            {
                Interact();
            }
            else
            {
                GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
                    this,
                    &AFirstPersonCharacter::Interact,
                    TargetInteractable-> InteractableData.InteractionDuration,
                    false);
            }
        }
    }
}

//Ends Interaction after Begin Interact and interact is called, Called upon in ETrigger movement for enhanced input actions
void AFirstPersonCharacter::EndInteract()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    if(IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->EndInteract();
    }
}

void AFirstPersonCharacter::Interact()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

    if(IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->Interact(this);
    }
}




