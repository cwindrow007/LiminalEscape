// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "BackroomsGame/Public/Components/InventoryComponent.h"

//Constructor
UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
	
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
	
}

//Copy items
UItemBase* UItemBase::CreateItemCopy() 
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy -> ID = this -> ID;
	ItemCopy -> Quantity = this -> Quantity;
	ItemCopy -> ItemQuality = this -> ItemQuality;
	ItemCopy -> ItemType = this -> ItemType;
	ItemCopy -> TextDescription = this -> TextDescription;
	ItemCopy ->ItemStatistics = this -> ItemStatistics;
	ItemCopy -> AssetData = this -> AssetData;
	ItemCopy -> bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize: 1 );

		if(OwningInventory)
		{
			if(Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
			
		}
		
	}
	
}

void UItemBase::Use(AFirstPersonCharacter* Character)
{
	
}



