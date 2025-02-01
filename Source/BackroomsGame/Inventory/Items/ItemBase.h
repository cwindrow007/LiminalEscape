// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BackroomsGame/Character/FirstPersonCharacter.h"
#include "BackroomsGame/Inventory/Data/ItemDataStructs.h"
#include "ItemBase.generated.h"



UCLASS()
class BACKROOMSGAME_API UItemBase : public UObject
{
	GENERATED_BODY()


public:
	//=============================================================================
	//PROPERTIES AND VARIABLES FOR INVSYS
	//=============================================================================

	//UPROPERTY()
	//UInventoryComponent* OwningInventory;
	
	UPROPERTY(EditAnywhere, Category = "Item Base", meta = (UIMin = 1, UIMax = 5));
	int32 Quantity;
	
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextDescription;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;

	//=============================================================================
	// FUNCTIONS
	//=============================================================================

	//Constructor For Accessing Items
	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy();

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AFirstPersonCharacter* Character);

protected:
//Does ID match Other ID
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
	
	
};
