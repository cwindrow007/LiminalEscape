// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputAction.h"
#include "GameSettingsManager.generated.h"

/**
 * 
 */

UCLASS()
class BACKROOMSGAME_API UGameSettingsManager : public UObject
{
	GENERATED_BODY()

public:

	UGameSettingsManager();

	void SaveSettings();
	void LoadSettings();

	//Sens
	void SetSens(float NewSens);
	float GetSens()const;
	
//Graphics settings
	void SetFullScreenMode(EWindowMode::Type NewMode);
	EWindowMode::Type GetFullScreenMode() const;

	void SetResolution(FIntPoint NewResolution);
	FIntPoint GetResolution() const;

	void SetVSync(bool bEnable);
	bool GetVsync() const;

	void SetFPSLimit(float NewLimit);
	float GetFpsLimit() const;

	void SetRTX(bool bEnable);
	bool GetRTX() const;

	void SetGamma(float NewGamma);
	float GetGamma() const;

	void SetMotionBlur(bool bEnable);
	bool GetMotionBlur() const;

	//Audio Settings
	void SetMasterVolume(float Volume);
	float GetMasterVolume() const;

	void SetMusicVolume(float Volume);
	float GetMusicVolume() const;

	void SetSFXVolume(float Volume);
	float GetSFXVolume() const;

	//In-Game Settings

	void SetShowCharacterMesh(bool bShow);
	bool GetShowCharacterMesh() const;

	void SetFOV(float FieldOfView);
	float GetFOV() const;

	void SetHeadBob(bool bEnable);
	bool GetHeadBob() const;

	void SetCameraZoom(bool bEnable);
	bool GetCameraZoom() const;

	void SetVHSEffect(bool bEnable);
	bool GetVHSEffect() const;

	//Controls Settings
	void SetKeyBinds(FName ActionName, FKey Key);
	FKey GetKeyBinds(FName ActionName) const;



private:
	//Graphics Settings
	EWindowMode::Type FullScreenMode;
	FIntPoint Resolution;
	bool bVSync;
	float FPSLimit;
	int32 GraphicsQuality;
	bool bRTX;
	float Gamma;
	bool bMotionBlur;

	//Audio Settings
	float MasterVolume;
	float MusicVolume;
	float SFXVolume;

	//In-Game Settings
	bool bShowCharacterMesh;
	float FOV;
	bool bHeadBob;
	bool bCameraZoom;
	bool bVHSEffect;

	//Control Settings
	TMap<FName, FKey> KeyBinds;

	//Sens
	float Sensitvity;

	//References to UINputAction Assets
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookUpAction;
	
};
