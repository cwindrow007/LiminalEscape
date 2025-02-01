// Fill out your copyright notice in the Description page of Project Settings.


#include "BackroomsGame/World/Pickup.h"
#include "BackroomsGame/Inventory/Items/ItemBase.h"
#include "BackroomsGame/Menus/InventoryWidget/InventoryPanel.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("Pickup Mesh");
	//PickupMesh->SetSimulate //This line is incase we want item physics, pref not for now maybe in the future =)
	SetRootComponent(PickupMesh);

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
	
}


void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if(ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData ->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData ->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextDescription = ItemData->TextDescription;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableDate();
	}
	
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	UpdateInteractableDate();
	
}

void APickup::UpdateInteractableDate()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextDescription.InteractionText;
	InstanceInteractableData.Name = ItemReference -> TextDescription.Name;
	InstanceInteractableData.Quantity = ItemReference ->Quantity;
	InteractableData = InstanceInteractableData;
}


void APickup::BeginFocus()
{
	if(PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if(PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(AFirstPersonCharacter* PlayerCharacter)
{
	if(PlayerCharacter)
	{
		takePickup(PlayerCharacter);
	}
	
}

void APickup::takePickup(const AFirstPersonCharacter* Taker)
{
	if(IsPendingKillPending())
	{
		if(ItemReference)
		{
			//if(UInventoryComponent* PlayerInventory = Taker->GetInventory())

			//Try to add item to player inventory based on result of the add operation
			//Adjust or destroy item
		}
	}
}









