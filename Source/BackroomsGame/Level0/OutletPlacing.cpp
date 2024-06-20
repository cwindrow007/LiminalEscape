// Fill out your copyright notice in the Description page of Project Settings.


#include "OutletPlacing.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AOutletPlacing::AOutletPlacing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOutletPlacing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOutletPlacing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOutletPlacing::PlaceOutlets()
{
	UWorld* World = GetWorld();
	if(!World) return;

	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{

		AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(Actor);
		if (StaticMeshActor && StaticMeshActor->ActorHasTag(WallTag))
		{
			
			int32 NumOutlets = FMath::RandRange(1,3);

			for(int32 i = 0; i <NumOutlets; i++)
			{
				FVector WallLocation = StaticMeshActor->GetActorLocation();
				FRotator WallRotation = StaticMeshActor->GetActorRotation();

				FVector RandomOffset = FVector(FMath::RandRange(-100.0f, 100.0f), 0.0f, HeightFromGround);
				FVector OutletPosition = WallLocation + StaticMeshActor->GetActorForwardVector() * RandomOffset.X + StaticMeshActor->GetActorUpVector() * RandomOffset.Z;

				FRotator OutLetRotation = WallRotation + FRotator(0.0f, 90.0f, 0.0f);

				FActorSpawnParameters SpawnParams;
				AStaticMeshActor* OutletActor = World->SpawnActor<AStaticMeshActor>(OutletMeshClass, OutletPosition, OutLetRotation, SpawnParams);

				if(OutletActor)
				{
					UStaticMeshComponent* MeshComponent = OutletActor->GetStaticMeshComponent();
					if(MeshComponent && !MeshComponent->GetStaticMesh())
					{
						MeshComponent->SetStaticMesh(OutletMesh);
					}
				}
			}
		}
	}
}


