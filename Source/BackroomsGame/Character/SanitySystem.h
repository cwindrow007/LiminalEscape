// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SanitySystem.generated.h"

/**
 * 
 */
UCLASS()
class BACKROOMSGAME_API USanitySystem : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(float InitialSanity, float DepletionRate);
	void StartSanityDepletion(UWorld* WorldContext);
	void StopSanityDepletion();
	float GetCurrentSanity() const;

private:
	void DecreaseSanity();

	float Sanity;
	float DepletionRate;
	FTimerHandle SanityTimerHandle;

	void OnSanityDepleted();
	
};
