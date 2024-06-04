// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Landscape.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Level94HouseGen.generated.h"



UCLASS()
class BACKROOMSGAME_API ALevel94HouseGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevel94HouseGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void PlaceHouses();
	bool GetLandscapeHeightAndNormalAtLocation(const FVector& Location, const ALandscape* Landscape, float& OutHeight, FVector& OutNormal) const;
	float GenerateRestrictedRotation(FRandomStream& RandomStream) const;
	bool IsLocationValid(const FVector& Location, float Radius) const;

	UPROPERTY(EditAnywhere, Category = "House")
	UStaticMesh* HouseMesh;

	UPROPERTY(EditAnywhere, Category = "House")
	int32 NumHouses;

	UPROPERTY(EditAnywhere, Category = "House")
	float Spacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "House", meta = (AllowPrivateAccess =  "true"))
	float HeightOffSet;

	UPROPERTY(EditAnywhere, Category = "House")
	int32 RandomSeed;

	TArray<ALandscape*> Landscapes;
	TArray<FVector> HouseLocations;
};
