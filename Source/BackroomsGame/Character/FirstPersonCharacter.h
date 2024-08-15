// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "FirstPersonCharacter.generated.h"

UCLASS()
class BACKROOMSGAME_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// Components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;


	//Movement Components

	UPROPERTY()
	UInputAction* IA_Jump;

	UPROPERTY()
	UInputAction* IA_Sprint;

	UPROPERTY()
	UInputAction* IA_MoveForward;

	UPROPERTY()
	UInputAction* IA_MoveRight;
	

	UPROPERTY()
	UInputAction* IA_Turn;

	UPROPERTY()
	UInputAction* IA_LookUp;

	// Enhanced Input Function Wrappers
	void MoveForwardEnhanced(const FInputActionValue& Value);
	void MoveRightEnhanced(const FInputActionValue& Value);
	void TurnEnhanced(const FInputActionValue& Value);
	void LookUpEnhanced(const FInputActionValue& Value);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	//Headbob Parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bEnableHeadbob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HeadbobFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HeadbobAmplitude;

	void UpdateHeadbob(float DeltaTime);
	void StartSprint();
	void StopSprint();

	//Camera Shake
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	
	void HandleCameraShake();

	UPROPERTY()
	class UGameSettingsManager* GameSettingsManager;

	float Sensitivity;

};
