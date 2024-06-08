// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Landscape.h"
#include "GameFramework/Actor.h"
#include "AutoPathPainter94.generated.h"

UCLASS()
class BACKROOMSGAME_API AAutoPathPainter94 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoPathPainter94();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Function to Paint Paths

	void PaintRandomPaths(ALandscape* Landscape, UMaterialInterface* DirtMaterial, int32 numPaths);

private:

	void PaintPathBewteenTwoPoints(const FVector& Start, const FVector& End, ALandscape* Landscape, UMaterialInterface* DirtMaterial);

	FVector GetRandomPointOnLandscape(ALandscape* Landscape, FRandomStream& RandomStream);
};
