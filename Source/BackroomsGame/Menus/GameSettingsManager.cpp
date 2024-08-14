// Fill out your copyright notice in the Description page of Project Settings.


// GameSettingsManager.cpp
#include "GameSettingsManager.h"
#include "Engine/Engine.h"
#include "GameFramework/InputSettings.h"

UGameSettingsManager::UGameSettingsManager()
{
    LoadSettings();

    static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Game/Inputs/IA_Jump"));
    if(JumpActionFinder.Succeeded())
    {
        JumpAction = JumpActionFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionFinder(TEXT("/Game/Input/IA_Sprint"));
    if (SprintActionFinder.Succeeded())
    {
        SprintAction = SprintActionFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> MoveForwardActionFinder(TEXT("/Game/Input/IA_MoveForward"));
    if (MoveForwardActionFinder.Succeeded())
    {
        MoveForwardAction = MoveForwardActionFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> MoveRightActionFinder(TEXT("/Game/Input/IA_MoveRight"));
    if (MoveRightActionFinder.Succeeded())
    {
        MoveRightAction = MoveRightActionFinder.Object;
    }
    
}

UInputAction* UGameSettingsManager::GetJumpAction() const
{
    return JumpAction;
}

UInputAction* UGameSettingsManager::GetSprintAction() const
{
    return SprintAction;
}

UInputAction* UGameSettingsManager::GetMoveForwardAction() const
{
    return MoveForwardAction;
}

UInputAction* UGameSettingsManager::GetMoveRightAction() const
{
    return MoveRightAction;
}





void UGameSettingsManager::SaveSettings()
{
    // Save settings to config or a custom file
}

void UGameSettingsManager::LoadSettings()
{
    // Load settings from config or a custom file
}

// Graphics Settings
void UGameSettingsManager::SetFullScreenMode(EWindowMode::Type NewMode)
{
    FullScreenMode = NewMode;
}


EWindowMode::Type UGameSettingsManager::GetFullScreenMode() const
{
    return FullScreenMode;
}

void UGameSettingsManager::SetResolution(FIntPoint NewResolution)
{
    Resolution = NewResolution;
}

FIntPoint UGameSettingsManager::GetResolution() const
{
    return Resolution;
}

void UGameSettingsManager::SetVSync(bool bEnable)
{
    bVSync = bEnable;
}

bool UGameSettingsManager::GetVsync() const
{
    return bVSync;
}


void UGameSettingsManager::SetFPSLimit(float NewLimit)
{
    FPSLimit = NewLimit;
}

float UGameSettingsManager::GetFpsLimit() const
{
    return  FPSLimit;
}


void UGameSettingsManager::SetRTX(bool bEnable)
{
    bRTX = bEnable;
}

bool UGameSettingsManager::GetRTX() const
{
    return bRTX;
}

void UGameSettingsManager::SetGamma(float NewGamma)
{
    Gamma = NewGamma;
}

float UGameSettingsManager::GetGamma() const
{
    return Gamma;
}

void UGameSettingsManager::SetMotionBlur(bool bEnable)
{
    bMotionBlur = bEnable;
}

bool UGameSettingsManager::GetMotionBlur() const
{
    return bMotionBlur;
}

// Audio Settings
void UGameSettingsManager::SetMasterVolume(float Volume)
{
    MasterVolume = Volume;
}

float UGameSettingsManager::GetMasterVolume() const
{
    return MasterVolume;
}

void UGameSettingsManager::SetMusicVolume(float Volume)
{
    MusicVolume = Volume;
}

float UGameSettingsManager::GetMusicVolume() const
{
    return MusicVolume;
}

void UGameSettingsManager::SetSFXVolume(float Volume)
{
    SFXVolume = Volume;
}

float UGameSettingsManager::GetSFXVolume() const
{
    return SFXVolume;
}

// In-game Settings
void UGameSettingsManager::SetShowCharacterMesh(bool bShow)
{
    bShowCharacterMesh = bShow;
}

bool UGameSettingsManager::GetShowCharacterMesh() const
{
    return bShowCharacterMesh;
}

void UGameSettingsManager::SetFOV(float FieldOfView)
{
    FOV = FieldOfView;
    if (GEngine)
    {
        if (GEngine->GetFirstLocalPlayerController(GetWorld()))
        {
            GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->SetFOV(FOV);
        }
    }
}

float UGameSettingsManager::GetFOV() const
{
    return FOV;
}

void UGameSettingsManager::SetHeadBob(bool bEnable)
{
    bHeadBob = bEnable;
}

bool UGameSettingsManager::GetHeadBob() const
{
    return bHeadBob;
}

void UGameSettingsManager::SetCameraZoom(bool bEnable)
{
    bCameraZoom = bEnable;
}

bool UGameSettingsManager::GetCameraZoom() const
{
    return bCameraZoom;
}

void UGameSettingsManager::SetVHSEffect(bool bEnable)
{
    bVHSEffect = bEnable;
}

bool UGameSettingsManager::GetVHSEffect() const
{
    return bVHSEffect;
}

//Sens
void UGameSettingsManager::SetSens(float NewSens)
{
    Sensitvity = NewSens;
    SaveSettings();
}

float UGameSettingsManager::GetSens() const
{
    return Sensitvity;
}



// Controls Settings
void UGameSettingsManager::SetKeyBinds(FName ActionName, FKey Key)
{
    KeyBinds.Add(ActionName, Key);
}

FKey UGameSettingsManager::GetKeyBinds(FName ActionName) const
{
    const FKey* Key = KeyBinds.Find(ActionName);
    return Key ? *Key : FKey();
}









