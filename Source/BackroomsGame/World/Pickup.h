// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "BackroomsGame/Interfaces/InteractionInterface.h"
#include "BackroomsGame/Inventory/Items/ItemBase.h"
#include "GameFramework/Actor.h"
#include "OverloadMacros.ush" 
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

	virtual void BeginFocus() override;
	
	virtual void EndFocus() override;


protected:
	//=============================================================================
	//PROPERTIES AND VARIABLES 
	//=============================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	//=============================================================================
	// Functions
	//=============================================================================
	

	virtual void BeginPlay() override;
	
	virtual void Interact(AFirstPersonCharacter* PlayerCharacter) override;
	void UpdateInteractableDate();

	void takePickup(const AFirstPersonCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
