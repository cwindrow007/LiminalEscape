// Fill out your copyright notice in the Description page of Project Settings.


#include"AutoPlaneMeshCutTool.h"
#include "ProceduralMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
AAutoPlaneMeshCutTool::AAutoPlaneMeshCutTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMesh;
	

}

// Called when the game starts or when spawned
void AAutoPlaneMeshCutTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAutoPlaneMeshCutTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AAutoPlaneMeshCutTool::PerformMeshCut()
{
	CutMesh();
}

void AAutoPlaneMeshCutTool::CutMesh()
{
	
}


