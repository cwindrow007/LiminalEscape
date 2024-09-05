// Fill out your copyright notice in the Description page of Project Settings.


#include "SanitySystem.h"
#include "TimerManager.h"
#include "ENgine/World.h"

void USanitySystem::Initialize(float InitialSanity, float InDepletionRate)
{
	Sanity = InitialSanity;
	DepletionRate = InDepletionRate;
	
}

void USanitySystem::StartSanityDepletion(UWorld* WorldContext)
{
	WorldContext->GetTimerManager().SetTimer(SanityTimerHandle, this, &USanitySystem::DecreaseSanity, 1.0f, true);
}

void USanitySystem::StopSanityDepletion()
{
	if(SanityTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SanityTimerHandle);
	}
}

void USanitySystem::DecreaseSanity()
{
	Sanity -= DepletionRate;
	if(Sanity <= 0.0f)
	{
		Sanity = 0.0f;
		OnSanityDepleted();
	}
}

void USanitySystem::OnSanityDepleted()
{
	UE_LOG(LogTemp, Warning, TEXT("Sanity = 0."));
}

float USanitySystem::GetCurrentSanity() const
{
	return Sanity;
}





