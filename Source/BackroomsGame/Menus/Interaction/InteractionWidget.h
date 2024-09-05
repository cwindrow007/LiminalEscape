// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

struct FInteractableData;
class AFirstPersonCharacter;
class UTextBlock;
class UProgressBar;
/**
 *
 * 
 */
UCLASS()
class BACKROOMSGAME_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	AFirstPersonCharacter* PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData);

protected:


	UPROPERTY(VisibleAnywhere, meta = (BindWidget),  Category = "Interaction Widget | Player Reference")
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget),  Category = "Interaction Widget | Player Reference")
	UTextBlock* ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Player Reference")
	UTextBlock* QuantityText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Player Reference")
	UTextBlock* KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Player Reference")
	UProgressBar* InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget),  Category = "Interaction Widget | Player Reference")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interaction Widget | Player Reference")
	float UpdateInteractionProgress();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

};
