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

    CeilingProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CeilingProdecuralMesh"));
    CeilingProceduralMesh->SetupAttachment(RootComponent);
    CeilingProceduralMesh->SetVisibility(false);
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
    if(LightFixture && CeilingMesh)
    {
        CutCeilingForLightFixture();
    }
}

void AAutoPlaneMeshCutTool::GetMeshData(TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
    if(CeilingMesh->GetStaticMesh())
    {
        const FStaticMeshLODResources& LODResources = CeilingMesh->GetStaticMesh()->GetLODForExport(0);
        const FPositionVertexBuffer& VertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
        const FRawStaticIndexBuffer& IndexBuffer = LODResources.IndexBuffer;

        for(uint32 i = 0; i < VertexBuffer.GetNumVertices(); i++)
        {
            Vertices.Add(FVector(VertexBuffer.VertexPosition(i)));
        }

        for (int32 i = 0; i < IndexBuffer.GetNumIndices(); i++)
        {
            Triangles.Add(IndexBuffer.GetIndex(i));
        }
    }
}

void AAutoPlaneMeshCutTool::CutCeilingForLightFixture()
{
    FVector Origin, BoxExtent;
    LightFixture->GetActorBounds(false, Origin, BoxExtent);

    //Get Ceiling Data
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    GetMeshData(Vertices, Triangles);

    TArray<FVector> NewVertices;
    TArray<int32> NewTriangles;

    for(int32 i = 0; i < Triangles.Num(); i+= 3)
    {
        FVector V0 = Vertices[Triangles[i]];
        FVector V1 = Vertices[Triangles[i + 1]];
        FVector V2 = Vertices[Triangles[i + 2]];

        bool Inside = true;
        for (const FVector& Vertex : {V0, V1, V2})
        {
            if(Vertex.X < Origin.X - BoxExtent.X || Vertex.X > Origin.X + BoxExtent.X||
               Vertex.Y < Origin.Y - BoxExtent.Y || Vertex.Y > Origin.Y + BoxExtent.Y||
               Vertex.Z < Origin.Z - BoxExtent.Z || Vertex.Z > Origin.Z + BoxExtent.Z)
            {
                Inside = false;
                break;
            }
        }
        if(Inside)
        {
            int32 Index0 = NewVertices.Add(V0);
            int32 Index1 = NewVertices.Add(V1);
            int32 Index2 = NewVertices.Add(V2);
            NewTriangles.Add(Index0);
            NewTriangles.Add(Index1);
            NewTriangles.Add(Index2);
        }
        
    }
    CreateMeshSection(NewVertices, NewTriangles);
}

void AAutoPlaneMeshCutTool::CreateMeshSection(TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
 
    CeilingProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2d>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);


    //Replace the Static Mesh with Procedural Mesh
    CeilingMesh->SetVisibility(false);

    CeilingProceduralMesh->SetVisibility(false);
}






