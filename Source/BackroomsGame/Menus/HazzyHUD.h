// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HazzyHUD.generated.h"

//Class Declarations
class UMainMenu;
class UInteractionWidget;
struct FInteractableData;

/**
 * 
 */
UCLASS()
class BACKROOMSGAME_API AHazzyHUD : public AHUD
{
	GENERATED_BODY()

public:

	//=============================================================================
	//PROPERTIES AND VARIABLES 
	//=============================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	//=============================================================================
	// FUNCTIONS
	//=============================================================================

	AHazzyHUD();

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);

protected:

	//=============================================================================
	//PROPERTIES AND VARIABLES 
	//=============================================================================

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;
	//=============================================================================
	//FUNCTIONS
	//=============================================================================
	virtual void BeginPlay() override;
};
