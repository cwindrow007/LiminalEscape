// Fill out your copyright notice in the Description page of Project Settings.


#include "Level0Monster.h"

#include "FunctionalAITest.h"
#include "FunctionalAITest.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"


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

}
//When PLayer is Seen Call Chase
void ALevel0Monster::OnSeePawn(APawn* Pawn)
{
	
	if(Pawn && !bPlayerDetected)
	{
		PlayerPawn = Pawn;
		bPlayerDetected = true;
		ChasePlayer(Pawn);
	}
}
//Triggers Roaming off of Begin play
void ALevel0Monster::StartRoaming()
{
	FVector RandomLocation = GetActorLocation() + FMath::VRand() * 1000.0f;
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController)
	{
		//Mass Error Here needing Full
		AIController->MoveToLocation(RandomLocation);
	}

	GetWorldTimerManager().SetTimer(RoamingTimerHandle, this, &ALevel0Monster::StartRoaming, 5.0f, false);
}

//Player Lock and Chase

void ALevel0Monster::ChasePlayer(APawn* Player)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController && Player)
	{
		AIController -> MoveToActor(Player);
	}
	GetWorldTimerManager().SetTimer(VisibilityCheckTimerHandle, this, &ALevel0Monster::CheckPlayerVisibility, 1.0f, true);
	
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
	FVector RandomLocation = PlayerLocation + FMath::VRand() * 2000.0f;

	SetActorLocation(RandomLocation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	bPlayerDetected = false;
	StartRoaming();
}






