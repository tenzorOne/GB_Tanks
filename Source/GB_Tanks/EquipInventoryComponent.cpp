// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipInventoryComponent.h"
#include "TowerMachineMaster.h"

UEquipInventoryComponent::UEquipInventoryComponent()
{
	EquipSlots.Add(0, EEquipSlot::EQ_CannonSlot);
	EquipSlots.Add(1, EEquipSlot::EQ_TrackSlot);
	EquipSlots.Add(2, EEquipSlot::EQ_ArmorSlot);
	EquipSlots.Add(3, EEquipSlot::EQ_TowerSlot);
	EquipSlots.Add(4, EEquipSlot::EQ_EngineSlot);

}

void UEquipInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{	
	if (Item.ID != NAME_None)
	{
		CurrentItem = const_cast<FInventorySlotInfo*>(&Item);
	}

	FInventorySlotInfo* EquippedItem = GetItem(SlotIndex);
	if (EquippedItem && CurrentItem->bUnequipProccess)
	{
		if (ATowerMachineMaster* PlayerCharacter = Cast<ATowerMachineMaster>(GetOwner()))
		{
			if (!CurrentItem->bUnequipAndStack)
			{
				CurrentItem->bUnequipProccess = false;
			}
			PlayerCharacter->UnequipItem(SlotIndex, Item.ID);
		}
	}

	Super::SetItem(SlotIndex, Item);

	if (!CurrentItem->bUnequipProccess && Item.ID != NAME_None)
	{
		if (ATowerMachineMaster* PlayerCharacter = Cast<ATowerMachineMaster>(GetOwner()))
		{
			CurrentItem->bUnequipProccess = true;
			PlayerCharacter->EquipItem(SlotIndex, Item.ID);
		}
	}

	CurrentItem->bUnequipAndStack = false;

}

int32 UEquipInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item)
{
	if (Item.Type != EItemType::IT_Equipment || !EquipSlots.Contains(SlotIndex) || EquipSlots.FindChecked(SlotIndex) != Item.EquipmentSlot)
	{
		return 0;
	}

	return 1;

}
