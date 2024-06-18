// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoPlaneMeshCutTool.h"

#include "NavigationSystemTypes.h"
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

void AAutoPlaneMeshCutTool::GetMeshData(TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    // Get the first procedural mesh section
    FProcMeshSection* MeshSection = ProceduralMesh->GetProcMeshSection(0);
    if (MeshSection)
    {
        // Copy vertices and triangles
        for (const FProcMeshVertex& Vertex : MeshSection->ProcVertexBuffer)
        {
            Vertices.Add(Vertex.Position);
        }
        Triangles = MeshSection->ProcIndexBuffer;
    }
}

void AAutoPlaneMeshCutTool::CutMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    GetMeshData(Vertices, Triangles);

    FPlane CuttingPlane(FVector(0, 0, 1), 0);

    TArray<FVector> NewVertices1, NewVertices2;
    TArray<int32> NewTriangles1, NewTriangles2;
    for (int32 i = 0; i < Triangles.Num(); i += 3)
    {
        FVector V0 = Vertices[Triangles[i]];
        FVector V1 = Vertices[Triangles[i + 1]];
        FVector V2 = Vertices[Triangles[i + 2]];

        if (CuttingPlane.PlaneDot(V0) >= 0 && CuttingPlane.PlaneDot(V1) >= 0 && CuttingPlane.PlaneDot(V2) >= 0)
        {
            int32 Index0 = NewVertices1.Add(V0);
            int32 Index1 = NewVertices1.Add(V1);
            int32 Index2 = NewVertices1.Add(V2);
            NewTriangles1.Add(Index0);
            NewTriangles1.Add(Index1);
            NewTriangles1.Add(Index2);
        }
        else if (CuttingPlane.PlaneDot(V0) <= 0 && CuttingPlane.PlaneDot(V1) <= 0 && CuttingPlane.PlaneDot(V2) <= 0)
        {
            int32 Index0 = NewVertices2.Add(V0);
            int32 Index1 = NewVertices2.Add(V1);
            int32 Index2 = NewVertices2.Add(V2);
            NewTriangles2.Add(Index0);
            NewTriangles2.Add(Index1);
            NewTriangles2.Add(Index2);
        }
        else
        {
            // Handle cases where the triangle is intersected by the plane (not implemented in this simplified example)
            // You will need to create new vertices at the intersection points and create new triangles accordingly.
        }
    }

    CreateMeshSection(NewVertices1, NewTriangles1);
    CreateMeshSection(NewVertices2, NewTriangles2);
}

void AAutoPlaneMeshCutTool::CreateMeshSection(TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    ProceduralMesh->ClearMeshSection(0);

    ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}
