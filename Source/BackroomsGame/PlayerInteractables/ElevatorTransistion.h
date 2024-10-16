// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorTransistion.generated.h"

UCLASS()
class BACKROOMSGAME_API AElevatorTransistion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorTransistion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	FString CorrectPasscode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	FVector TargetLocation;

};
