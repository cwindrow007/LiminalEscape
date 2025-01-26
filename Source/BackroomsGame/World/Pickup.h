// Fill out your copyright notice in the Description page of Project Settings.
/*

#pragma once

#include "CoreMinimal.h"
#include "BackroomsGame/Interfaces/InteractionInterface.h"
#include "BackroomsGame/Inventory/Items/ItemBase.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BACKROOMSGAME_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	//=============================================================================
	//PROPERTIES AND VARIABLES 
	//=============================================================================


	//=============================================================================
	// Functions
	//=============================================================================
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData(){return ItemReference; }

protected:
public:
	//=============================================================================
	//PROPERTIES AND VARIABLES 
	//=============================================================================
	UStaticMeshComponent* PickupMesh;
	
	UDataTable* ItemDataTable;

	FName DesiredItemID;

	UItemBase* ItemReference;

	int32 ItemQuantity;

	FInteractableData InstanceInteractableData;
	
	//=============================================================================
	// Functions
	//=============================================================================
	

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

};
*/