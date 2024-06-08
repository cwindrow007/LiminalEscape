// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoPathPainter94.h"
#include "Landscape.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/DecalComponent.h"
#include " Engine/World.h"

// Sets default values
AAutoPathPainter94::AAutoPathPainter94()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAutoPathPainter94::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAutoPathPainter94::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APathPainter::RandomPaths(ALandscape* Landscape, UMaterialInterface* PathMaterial, int32 numPaths)
{
	if(!Landscape|| !Pathmaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Landscape or path material available for path painting));
			return;
	}
}


