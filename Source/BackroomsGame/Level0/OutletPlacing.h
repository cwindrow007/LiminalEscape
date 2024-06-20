// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OutletPlacing.generated.h"

UCLASS()
class BACKROOMSGAME_API AOutletPlacing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOutletPlacing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Outlets")
	void PlaceOutlets();

private:
	UPROPERTY(EditAnywhere, Category = "Outlets")
	TSubclassOf<AStaticMeshActor> OutletMeshClass;

	UPROPERTY(EditAnywhere, Category = "Outlets")
	UStaticMesh* OutletMesh;

	UPROPERTY(EditAnywhere, Category = "Outlets")
	float HeightFromGround = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Outlets")
	FName WallTag;

};
