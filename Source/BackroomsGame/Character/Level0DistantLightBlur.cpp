// Fill out your copyright notice in the Description page of Project Settings.


#include "Level0DistantLightBlur.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevel0DistantLightBlur::ALevel0DistantLightBlur()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);

	PlayerCharacter = nullptr;
	MaxDistance = 1681.0f;
	LightMaterial = nullptr;
	DynamicMaterialInstance = nullptr;

}

// Called when the game starts or when spawned
void ALevel0DistantLightBlur::BeginPlay()
{
	Super::BeginPlay();

	if(LightMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(LightMaterial, this);
		if(TargetMesh)
		{
			TargetMesh->SetMaterial(0, DynamicMaterialInstance);
		}
	}

	if(!PlayerCharacter)
	{
		PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	
}

// Called every frame
void ALevel0DistantLightBlur::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBlurEffect();

}

void ALevel0DistantLightBlur::UpdateBlurEffect()
{
	if(!PlayerCharacter|| !DynamicMaterialInstance)
	{
		return;
	}
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector LightLocation = GetActorLocation();
	float Distance = FMath::Abs(PlayerLocation.Y - LightLocation.Y);

	float BlurAmount = 0.0f;
	if(Distance > MaxDistance)
	{
		BlurAmount = 1.0f;
	}
	else
	{
		BlurAmount = 1- (Distance/MaxDistance);
	}
	DynamicMaterialInstance->SetScalarParameterValue(TEXT("BlueAmount"), BlurAmount);
}


