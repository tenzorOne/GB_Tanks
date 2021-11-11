// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "EquipInventoryComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class GB_TANKS_API UEquipInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	UEquipInventoryComponent();

	void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item) override;
	int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) override;

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, EEquipSlot> EquipSlots;

	FInventorySlotInfo* CurrentItem;

};
