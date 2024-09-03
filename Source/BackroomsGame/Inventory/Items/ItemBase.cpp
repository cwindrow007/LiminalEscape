// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

//Constructor
UItemBase::UItemBase()
{
	
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
	

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.IsStackable ? NumericData.MaxStackSize: 1 );

		/*if(OwningInventory)
		{
			if(quantity <= 0)
			{
			owningInventory->RemoveItem(this);

			}
			
		}
		*/
	}
	
}

void UItemBase::Use(AFirstPersonCharacter* Character)
{
	
}



