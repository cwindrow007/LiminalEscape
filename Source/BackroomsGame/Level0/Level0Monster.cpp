// Fill out your copyright notice in the Description page of Project Settings.

//This animation is broken and needs a fix do this tmr.

#include "Level0Monster.h"

#include "AIController.h"
#include "TimerManager.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/PlayerController.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"

// Sets default values
ALevel0Monster::ALevel0Monster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ALevel0Monster::OnSeePawn);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALevel0Monster::OnPlayerDetected);

	bPlayerDetected = false;
	bIsRoaming = true;

	BaseWalkSpeed = 450.0f;
	OnPlayerDetectSpeed = 1200.0f;

	CurrentDirection = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ALevel0Monster::BeginPlay()
{
	Super::BeginPlay();
	StartRoaming();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	// Activate pawn sensing
	PawnSensingComponent->SetSensingInterval(0.5f);
	PawnSensingComponent->SetPeripheralVisionAngle(90.0f);
	PawnSensingComponent->SightRadius = 20000.0f;
}

// Called every frame
void ALevel0Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentSpeed = GetVelocity().Size();
	UE_LOG(LogTemp, Log, TEXT("AI Speed: %f"), CurrentSpeed);

	if(bPlayerDetected && PlayerPawn)
	{
		MoveToPlayer(DeltaTime);
	}

	else if(bIsRoaming)
	{
		FVector NewVelocity = CurrentDirection * BaseWalkSpeed;
		GetCharacterMovement()->Velocity = NewVelocity;
		
		AddMovementInput(CurrentDirection);

		if(GetVelocity().Size() > 0)
		{
			FRotator TargetRotation = GetVelocity().Rotation();
			FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 3.0f);
			SetActorRotation(SmoothRotation);
		}
	}
}

// When Player is Seen Call Chase
void ALevel0Monster::OnSeePawn(APawn* Pawn)
{
	if (Pawn && !bPlayerDetected)
	{
		PlayerPawn = Pawn;
		bPlayerDetected = true;
		bIsRoaming = false;
		GetCharacterMovement()->MaxWalkSpeed = OnPlayerDetectSpeed;
		
		
		UE_LOG(LogTemp, Warning, TEXT("Player detected!"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player detected!"));
		}
		GetWorldTimerManager().SetTimer(VisibilityCheckTimerHandle, this, &ALevel0Monster::CheckPlayerVisibility, 1.0f, true);
	}
}

void ALevel0Monster::MoveToPlayer(float DeltaTime)
{
	if(PlayerPawn)
	{
		FVector DirectionToPlayer = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector RunVelocity= DirectionToPlayer * OnPlayerDetectSpeed;
		GetCharacterMovement()->Velocity = RunVelocity;

		AddMovementInput(DirectionToPlayer, 1.0f);

		FRotator TargetRotation = DirectionToPlayer.Rotation();
		FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
		SetActorRotation(SmoothRotation);
		
	}
}


// Overlap Begin function
void ALevel0Monster::OnPlayerDetected(UPrimitiveComponent* OvdCerlappeomp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor == PlayerPawn)
	{
		TeleportToRandomLocation();
	}
}

void ALevel0Monster::PickNewDirection()
{
	FRotator RandomRotation = FRotator(0, FMath::RandRange(0.f, 360.f), 0);
	CurrentDirection = RandomRotation.Vector();
	bIsRoaming = true;
}


// Triggers Roaming off of Begin play
void ALevel0Monster::StartRoaming()
{
	PickNewDirection();
	bIsRoaming = true;

}

void ALevel0Monster::CheckPlayerVisibility()
{
	if (!PawnSensingComponent->HasLineOfSightTo(PlayerPawn))
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetWorldTimerManager().ClearTimer(VisibilityCheckTimerHandle);
		GetWorldTimerManager().SetTimer(DespawnTimerHandle, this, &ALevel0Monster::Despawn, 3.0f, false);

		// Print to log and on-screen
		UE_LOG(LogTemp, Warning, TEXT("Player lost!"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player lost!"));
		}
	}
}

void ALevel0Monster::Despawn()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ALevel0Monster::Respawn, 10.0f, false);
}

void ALevel0Monster::Respawn()
{
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FVector RandomLocation;
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation NavLocation;
		if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, 4000.0f, NavLocation))
		{
			RandomLocation = NavLocation.Location;
		}
	}

	SetActorLocation(RandomLocation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	bPlayerDetected = false;
	StartRoaming();
}

void ALevel0Monster::TeleportToRandomLocation()
{
	FVector RandomLocation;
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation NavLocation;
		if (NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), 4000.0f, NavLocation))
		{
			RandomLocation = NavLocation.Location;
		}
	}
	SetActorLocation(RandomLocation);
	UE_LOG(LogTemp, Warning, TEXT("Teleported to %s:"), *RandomLocation.ToString());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Teleported to %s"), *RandomLocation.ToString()));
	}
}

void ALevel0Monster::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other && Other != PlayerPawn)
	{
		PickNewDirection();
	}
}
