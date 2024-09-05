// Fill out your copyright notice in the Description page of Project Settings.


#include "HazzyHUD.h"
#include "BackroomsGame/Menus/MainMenu.h"
#include "BackroomsGame/Menus/Interaction/InteractionWidget.h"

AHazzyHUD::AHazzyHUD()
{
	
}

void AHazzyHUD::BeginPlay()
{
	Super::BeginPlay();
	if(MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport();
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport();
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	
}

void AHazzyHUD::DisplayMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AHazzyHUD::HideMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AHazzyHUD::ShowInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AHazzyHUD::HideInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}


void AHazzyHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		//InteractionWidget->UpdateWidget(InteractableData);
	}
}