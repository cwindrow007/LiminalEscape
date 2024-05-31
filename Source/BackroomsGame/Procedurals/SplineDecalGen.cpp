#include "SplineDecalGen.h"
#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Landscape.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"

// Sets default values
ASplineDecalGen::ASplineDecalGen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

// Called when the game starts or when spawned
void ASplineDecalGen::BeginPlay()
{
	Super::BeginPlay();

	AdjustSplineToLandscape();
}

// Called every frame
void ASplineDecalGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASplineDecalGen::GenerateDecals()
{
	if (!DecalMaterial || !SplineComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("DecalMaterial or SplineComponent is not set!"));
		return;
	}

	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	float Distance = 0.0f;
	float RowOffset = 0.0f;

	for (int32 Row = 0; Row < NumberOfRows; ++Row)
	{
		while (Distance < SplineComponent->GetSplineLength())
		{
			FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

			// Offset location for additional rows
			FVector RowLocation = Location + Rotation.RotateVector(FVector(RowOffset, 0.0f, 0.0f));

			FActorSpawnParameters SpawnParams;
			ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(RowLocation, Rotation, SpawnParams);
			if (Decal)
			{
				Decal->SetDecalMaterial(DecalMaterial);
				Decal->SetLifeSpan(0); // Set lifespan to 0 to make it permanent

				// Access the Decal component and set its size
				if (UDecalComponent* DecalComponent = Decal->GetDecal())
				{
					DecalComponent->DecalSize = FVector(DecalLength, DecalLength, 1.0f);
				}
			}

			Distance += DecalLength + GapBetweenDecals;
		}

		Distance = 0.0f; // Reset distance for next row
		RowOffset += DecalLength + GapBetweenDecals; // Update offset for next row
	}
}

void ASplineDecalGen::AdjustSplineToLandscape()
{
	if (!Landscape)
	{
		UE_LOG(LogTemp, Warning, TEXT("Landscape reference is not set!"));
		return;
	}

	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	for (int32 i = 0; i < NumPoints; ++i)
	{
		FVector SplinePointLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector StartLocation = SplinePointLocation + FVector(0.0f, 0.0f, 1000.0f); // Start trace above the landscape
		FVector EndLocation = SplinePointLocation - FVector(0.0f, 0.0f, 1000.0f);   // End trace below the landscape

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		// Perform a line trace to detect the landscape surface
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
		{
			if (HitResult.GetActor() && HitResult.GetActor()->IsA(ALandscape::StaticClass()))
			{
				FVector NewLocation = HitResult.Location;
				SplineComponent->SetLocationAtSplinePoint(i, NewLocation, ESplineCoordinateSpace::World, true);
			}
		}
	}
}