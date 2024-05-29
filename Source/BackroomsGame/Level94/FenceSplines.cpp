// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceSplines.h"

#include "Editor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

// Sets default values
AFenceSplines::AFenceSplines()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if(SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}
}
void AFenceSplines::OnConstruction(const FTransform& Transform)
{
	if(SplineMeshMap.Num() > 0)
	{
		FSplineMeshDetails* DefaultMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::DEFAULT))
		{
			DefaultMeshDetails = SplineMeshMap.Find(ESplineMeshType::DEFAULT);
			
		}
		else
		{
			//no default = no game
			return;
		}
		
		FSplineMeshDetails* StartMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::START))
		{
			StartMeshDetails = SplineMeshMap.Find(ESplineMeshType::START);
			
		}
		FSplineMeshDetails* EndMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::END))
		{
			EndMeshDetails = SplineMeshMap.Find(ESplineMeshType::END);
			
		}
		for(int SplineCount = 0; SplineCount< (SplineComponent-> GetNumberOfSplinePoints()-1);SplineCount++)
		{
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);
			SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
			SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

			const FVector StartPoint = SplineComponent ->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
			const FVector StartTangent = SplineComponent -> GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
			const FVector EndPoint = SplineComponent -> GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
			const FVector EndTangent = SplineComponent -> GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);

			SplineMeshComponent -> SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

			SplineMeshComponent -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			
			//Start
			if(StartMeshDetails && StartMeshDetails -> Mesh&& SplineCount == 0)
			{
				SplineMeshComponent->SetStaticMesh(StartMeshDetails->Mesh);
				SplineMeshComponent->SetForwardAxis(StartMeshDetails->ForwardAxis);
			}
			//End
			else if (EndMeshDetails && EndMeshDetails->Mesh && SplineComponent->GetNumberOfSplinePoints() > 2 && SplineCount == (SplineComponent->GetNumberOfSplinePoints()-2))
			{
				SplineMeshComponent->SetStaticMesh(EndMeshDetails->Mesh);
				SplineMeshComponent->SetForwardAxis(EndMeshDetails->ForwardAxis);
			}
			//Middle
			else
			{
				if(DefaultMeshDetails ->Mesh)
				{
					
					SplineMeshComponent->SetStaticMesh(DefaultMeshDetails->Mesh);
					SplineMeshComponent->SetForwardAxis(DefaultMeshDetails->ForwardAxis);
					

					if(DefaultMeshDetails->AlternateMaterial && SplineCount > 0 && SplineCount % 2 == 0)
					{
						SplineMeshComponent -> SetMaterial(0, DefaultMeshDetails->AlternateMaterial);
					}
					else if (DefaultMeshDetails->DefaultMaterial)
					{
						SplineMeshComponent -> SetMaterial(0, DefaultMeshDetails->DefaultMaterial);
					}
				}
			}
		}
	}
	
}


// Called when the game starts or when spawned
void AFenceSplines::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFenceSplines::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

