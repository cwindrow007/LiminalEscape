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
        
        Triangles.Append(MeshSection->ProcIndexBuffer);
    }
}

FVector AAutoPlaneMeshCutTool::GetPlaneLineIntersection(const FPlane& Plane, const FVector& LineStart, const FVector& LineEnd)
{
    FVector LineDir = LineEnd - LineStart;
    float T = (-Plane.W - FVector::DotProduct(Plane.GetNormal(), LineStart))/ FVector::DotProduct(Plane.GetNormal(), LineDir);
    return LineStart + T * LineDir;
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

        float D0 = CuttingPlane.PlaneDot(V0);
        float D1 = CuttingPlane.PlaneDot(V1);
        float D2 = CuttingPlane.PlaneDot(V2);

        if (D0 >= 0 && D1 >= 0 && D2 >= 0)
        {
            int32 Index0 = NewVertices1.Add(V0);
            int32 Index1 = NewVertices1.Add(V1);
            int32 Index2 = NewVertices1.Add(V2);
            NewTriangles1.Add(Index0);
            NewTriangles1.Add(Index1);
            NewTriangles1.Add(Index2);
        }
        else if (D0 <= 0 && D1 <= 0 && D2 <= 0)
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
            FVector AboveVert1, AboveVert2, BelowVert;
            if(D0 >= 0) {AboveVert1 = V0; if (D1>= 0) {AboveVert2 = V2; BelowVert = V2; }else{ AboveVert2 = V2; BelowVert = V1;}}
            else{AboveVert1 = V1; AboveVert2 = V2; BelowVert = V0;}

            //Calc Intersection Points

            FVector Intersect1 = GetPlaneLineIntersection(CuttingPlane, BelowVert, AboveVert1);
            FVector Intersect2 = GetPlaneLineIntersection(CuttingPlane, BelowVert, AboveVert2);

            int32 IndexAbove1 = NewVertices1.Add(AboveVert1);
            int32 IndexAbove2 = NewVertices1.Add(AboveVert2);
            int32 IndexIntersect1 = NewVertices1.Add(Intersect1);
            int32 IndexIntersect2= NewVertices1.Add(Intersect2);

            NewTriangles1.Add(IndexAbove1);
            NewTriangles1.Add(IndexIntersect1);
            NewTriangles1.Add(IndexIntersect2);

            NewTriangles1.Add(IndexAbove1);
            NewTriangles1.Add(IndexIntersect2);
            NewTriangles1.Add(IndexAbove2);

            //Add Vertices and triangels to the below plane sections

            int32 IndexBelow = NewVertices2.Add(BelowVert);
            int32 IndexIntersect1Below = NewVertices2.Add(Intersect1);
            int32 IndexIntersect2Below = NewVertices2.Add(Intersect2);

            NewTriangles2.Add(IndexBelow);
            NewTriangles2.Add(IndexIntersect1Below);
            NewTriangles2.Add(IndexIntersect2Below);
            
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
