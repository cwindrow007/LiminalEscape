#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Engine/DecalActor.h"
#include "Landscape.h"
#include "SplineDecalGen.generated.h"

UCLASS()
class BACKROOMSGAME_API ASplineDecalGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineDecalGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	//Decal Material Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	UMaterialInterface* DecalMaterial;
	
	//Length Of Decals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	float DecalLength;

	//Gaps between the Decals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	float GapBetweenDecals;

	//Number of rows of Decals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	int32 NumberOfRows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Decal")
	FVector DecalSize;



	



private:
	//Function to adjust spline points based on landscape
	void AdjustSplineToLandscape();
	void GenerateDecals();

	//Spline Component Reference
	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComponent;

	//Landscape reference for floor
	UPROPERTY(EditAnywhere, Category = "Landscape")
	ALandscape* Landscape;
};