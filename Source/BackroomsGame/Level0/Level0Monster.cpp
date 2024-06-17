// Fill out your copyright notice in the Description page of Project Settings.


#include "Level0Monster.h"

#include "AIController.h"
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

// Sets default values
ALevel0Monster::ALevel0Monster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ALevel0Monster::OnSeePawn);

	bPlayerDetected = false;

}

// Called when the game starts or when spawned
void ALevel0Monster::BeginPlay()
{
	Super::BeginPlay();
	StartRoaming();
	
}

// Called every frame
void ALevel0Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bPlayerDetected && PlayerPawn)
	{
		AAIController* AIContoller = Cast<AAIController>(GetController());
		if(AIContoller)
		{
			AIContoller->MoveToActor(PlayerPawn);
		}
	}
	else
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController && AIController->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Idle)
		{
			StartRoaming();
		}
	}
}


//When PLayer is Seen Call Chase
void ALevel0Monster::OnSeePawn(APawn* Pawn)
{
	
	if(Pawn && !bPlayerDetected)
	{
		PlayerPawn = Pawn;
		bPlayerDetected = true;
		GetWorldTimerManager().ClearTimer(VisibilityCheckTimerHandle);
		GetWorldTimerManager().SetTimer(VisibilityCheckTimerHandle, this, &ALevel0Monster::CheckPlayerVisibility, 1.0f, true);
	}
}
//Triggers Roaming off of Begin play
void ALevel0Monster::StartRoaming()
{
	FVector RandomLocation = GetActorLocation() + FMath::VRand() * 1000.0f;
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController)
	{
		AIController->MoveToLocation(RandomLocation);
	}

	GetWorldTimerManager().SetTimer(RoamingTimerHandle, this, &ALevel0Monster::StartRoaming, 5.0f, false);
}

void ALevel0Monster::CheckPlayerVisibility()
{
	if(!PawnSensingComponent->HasLineOfSightTo(PlayerPawn))
	{
		GetWorldTimerManager().ClearTimer(VisibilityCheckTimerHandle);
		GetWorldTimerManager().SetTimer(DespawnTimerHandle, this, &ALevel0Monster::Despawn, 3.0f, false);
	}
}

void ALevel0Monster::Despawn()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ALevel0Monster::Respawn, 5.0f, false);
}
void ALevel0Monster::Respawn()
{
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FVector RandomLocation;
	if(UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation NavLocation;
		if(NavSys->GetRandomPointInNavigableRadius(PlayerLocation, 2000.0f, NavLocation))
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
	if(UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation NavLocation;
		if(NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), 2000.0f, NavLocation))
		{
			RandomLocation = NavLocation.Location;
		}
	}
	SetActorLocation(RandomLocation);
	UE_LOG(LogTemp, Warning, TEXT("Teleported to %s:"), *RandomLocation.ToString());
}
void ALevel0Monster::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if(Other && Other == PlayerPawn)
	{
		TeleportToRandomLocation();
	}
}







