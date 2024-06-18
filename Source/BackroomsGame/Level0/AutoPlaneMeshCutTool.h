// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "AutoPlaneMeshCutTool.generated.h"

UCLASS()
class BACKROOMSGAME_API AAutoPlaneMeshCutTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoPlaneMeshCutTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	void PerformMeshCut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	AActor* LightFixture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* CeilingMesh;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* CeilingProceduralMesh;

private:
	
	

	void GetMeshData(TArray<FVector>& Vertices, TArray<int32>& Triangles);
	void CreateMeshSection(TArray<FVector>& Vertices, TArray<int32>& Triangles);

	void CutCeilingForLightFixture();

};
