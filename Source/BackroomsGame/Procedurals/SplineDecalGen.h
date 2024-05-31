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

	//Spline Component Reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

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

	//Landscape reference for floor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape")
	ALandscape* Landscape;

	//Function to generate decals along the Spline
	UFUNCTION(BlueprintCallable, Category = "Generation")
	void GenerateDecals();

private:
	//Function to adjust spline points based on landscape
	void AdjustSplineToLandscape();
};