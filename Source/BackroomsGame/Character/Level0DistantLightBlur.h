// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Level0DistantLightBlur.generated.h"

UCLASS()
class BACKROOMSGAME_API ALevel0DistantLightBlur : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevel0DistantLightBlur();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	float MaxDistance;

	UFUNCTION(BlueprintCallable, Category = "Light")
	void UpdateBlurEffect();
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TargetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* LightMaterial;

	UMaterialInstanceDynamic* DynamicMaterialInstance;
};


