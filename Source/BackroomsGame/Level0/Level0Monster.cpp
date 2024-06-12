// Fill out your copyright notice in the Description page of Project Settings.


#include "Level0Monster.h"

// Sets default values
ALevel0Monster::ALevel0Monster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevel0Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevel0Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALevel0Monster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

