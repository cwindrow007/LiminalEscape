#include "Level94HouseGen.h"
#include "Engine/World.h"
#include "Landscape.h"
#include "LandscapeComponent.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"


// Sets default values
ALevel94HouseGen::ALevel94HouseGen()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize default values
    HeightOffSet = 100.0f;
    RandomSeed = 12345; // Set a default seed value
}

// Called when the game starts or when spawned
void ALevel94HouseGen::BeginPlay()
{
    Super::BeginPlay();

    // Find the landscapes in the level
    for (TActorIterator<ALandscape> It(GetWorld()); It; ++It)
    {
        Landscapes.Add(*It);
    }
    if (Landscapes.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Found %d Landscapes"), Landscapes.Num());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Landscape Found in this level"));
    }
    if (!HouseMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("No House Mesh Assigned"));
    }

    PlaceHouses();
}

// Called every frame
void ALevel94HouseGen::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALevel94HouseGen::PlaceHouses()
{
    if (Landscapes.Num() == 0 || !HouseMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("No landscape or house mesh found"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("Placing Houses"));

    FRandomStream RandomStream(RandomSeed);
    
    int32 TotalLandscapes = Landscapes.Num();
    int32 HousesPerLandScape = NumHouses / TotalLandscapes;
    
    for (int32 LandscapeIndex = 0; LandscapeIndex < TotalLandscapes; ++LandscapeIndex)
    {
        ALandscape* Landscape = Landscapes[LandscapeIndex];
        if (!Landscape) continue;

        int32 HousePerRow = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(NumHouses)));
        FVector LandscapeOrigin;
        FVector LandscapeBoundsExtent;
        Landscape->GetActorBounds(false, LandscapeOrigin, LandscapeBoundsExtent);

        FVector LandscapeMin = LandscapeOrigin - LandscapeBoundsExtent;
        FVector LandscapeMax = LandscapeOrigin + LandscapeBoundsExtent;

        for(int32 HouseIndex = 0; HouseIndex < HousesPerLandScape; ++HouseIndex)
        {
            FVector Location;
            Location.X = RandomStream.FRandRange(LandscapeMin.X, LandscapeMax.X);
            Location.Y = RandomStream.FRandRange(LandscapeMin.Y, LandscapeMax.Y);

            float Height;
            FVector Normal;
            if (GetLandscapeHeightAndNormalAtLocation(Location, Landscape, Height, Normal))
            {
                Location.Z = Height + HeightOffSet;

                // Generate a rotation based on the normal of the landscape
                FRotator Rotation = FRotationMatrix::MakeFromZ(Normal).Rotator();
                Rotation.Yaw = RandomStream.FRandRange(0.0f, 360.0f);

                UE_LOG(LogTemp, Log, TEXT("House %d Location: %s on Landscape %s with Rotation: %s"), HouseIndex, *Location.ToString(), *Landscape->GetName(), *Rotation.ToString());

                FActorSpawnParameters SpawnParams;
                AStaticMeshActor* HouseActor = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation, SpawnParams);
                if (HouseActor)
                {
                    UStaticMeshComponent* MeshComponent = HouseActor->GetStaticMeshComponent();
                    if (MeshComponent)
                    {
                        MeshComponent->SetStaticMesh(HouseMesh);
                        UE_LOG(LogTemp, Log, TEXT("House %d spawned successfully on %s"), HouseIndex, *Landscape->GetName());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("House %d mesh component not found"), HouseIndex);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("House %d not spawned"), HouseIndex);
                }
            }
        }
    }
}

bool ALevel94HouseGen::GetLandscapeHeightAndNormalAtLocation(const FVector& Location, const ALandscape* Landscape, float& OutHeight, FVector& OutNormal) const
{
    if (!Landscape)
    {
        UE_LOG(LogTemp, Warning, TEXT("No landscape available for height calculation"));
        return false;
    }

    FVector StartLocation = FVector(Location.X, Location.Y, 10000.0f);
    FVector EndLocation = FVector(Location.X, Location.Y, -10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
    {
        OutHeight = HitResult.Location.Z;
        OutNormal = HitResult.Normal;
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace did not hit landscape"));
    }

    OutHeight = 0.0f;
    OutNormal = FVector::UpVector;
    return false;
}

float ALevel94HouseGen::GenerateRestrictedRotation(FRandomStream& RandomStream) const
{
    float Rotation = RandomStream.FRandRange(0.0f, 360.0f);
    while (Rotation > 30.0f && Rotation < 330.0f)
    {
        Rotation = RandomStream.FRandRange(0.0f, 360.0f);
    }
    return Rotation;
}
