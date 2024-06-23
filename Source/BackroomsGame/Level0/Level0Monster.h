// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Level0Monster.generated.h"



UCLASS()
class BACKROOMSGAME_API ALevel0Monster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALevel0Monster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void StartRoaming();
	void CheckPlayerVisibility();
	void Despawn();
	void Respawn();
	void TeleportToRandomLocation();

	UFUNCTION()
	void OnPlayerDetected(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void KillPlayer(AActor* PlayerActor);

private:
	FTimerHandle RoamingTimerHandle;
	FTimerHandle VisibilityCheckTimerHandle;
	FTimerHandle DespawnTimerHandle;
	FTimerHandle RespawnTimerHandle;

	APawn* PlayerPawn;
	bool bPlayerDetected;

protected:
	virtual  void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	

};
