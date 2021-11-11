
#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "InventoryCellWidget.h"
#include <Components/OverlaySlot.h>


UInventoryManagerComponent::UInventoryManagerComponent()
{


}

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;

	if (InventoryComponent && InventoryItemsData)
	{
		ensure(InventoryWidgetClass);
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
		InventoryWidget->RepresentedInventory = InventoryComponent;
		InventoryWidget->AddToViewport();

		InventoryWidget->Init(FMath::Max(InventoryComponent->GetItemsNum(), MinInventorySize));

		for (auto& Item : InventoryComponent->GetItems())
		{
			FInventoryItemInfo* ItemData = GetItemData(Item.Value.ID);
			if (ItemData)
			{
				ItemData->Icon.LoadSynchronous();
				InventoryWidget->AddItem(Item.Value, *ItemData, GetItemIconTexture(InventoryItemsData, Item.Value.ID), Item.Key);
			}
		}
	}

}

void UInventoryManagerComponent::InitEquipment(UInventoryComponent* InInventoryComponent)
{
	ensure(EquipInventoryWidgetClass);
	EquipInventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), EquipInventoryWidgetClass);
	EquipInventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
	EquipInventoryWidget->RepresentedInventory = InInventoryComponent;
	EquipInventoryWidget->AddToViewport();

}

FInventoryItemInfo* UInventoryManagerComponent::GetItemData(FName ItemID)
{
	return InventoryItemsData ? InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, "") : nullptr;

}

UTexture2D* UInventoryManagerComponent::GetItemIconTexture(UDataTable* DataTable, FName IconID)
{
	FInventorySlotInfo SlotInfo;
	SlotInfo.ID = IconID;

	return SlotInfo.GetIconTexture(DataTable, IconID);

}

EItemRarity UInventoryManagerComponent::GetItemRarity(FName ItemID)
{
	if (InventoryItemsData)
	{
		return InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, "")->Rarity;
	}
	else
	{
		return EItemRarity::IR_None;
	}

}

void UInventoryManagerComponent::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{
	if (DraggedFrom == nullptr || DroppedTo == nullptr)
	{
		return;
	}

	UInventoryComponent* FromInventory = DraggedFrom->ParentInventoryWidget->RepresentedInventory;
	UInventoryComponent* ToInventory = DroppedTo->ParentInventoryWidget->RepresentedInventory;

	if (FromInventory == nullptr || ToInventory == nullptr)
	{
		return;
	}

	InventoryComponent->SetItem(DraggedFrom->IndexInInventory, DroppedTo->GetItem());
	InventoryComponent->SetItem(DroppedTo->IndexInInventory, DraggedFrom->GetItem());

	FInventorySlotInfo FromSlotItem = DraggedFrom->GetItem();
	FInventorySlotInfo ToSlotItem = DroppedTo->GetItem();

	if (FromSlotItem.Count <= 0)
	{
		return;
	}
	
	FInventoryItemInfo* FromInfoItem = GetItemData(FromSlotItem.ID);
	FInventoryItemInfo* ToInfoItem = GetItemData(ToSlotItem.ID);

	if (FromInfoItem == nullptr || (ToSlotItem.ID != NAME_None && ToInfoItem == nullptr))
	{
		return;
	}

	int32 MaxCount = ToInventory->GetMaxItemAmount(DroppedTo->IndexInInventory, *FromInfoItem);
	if (MaxCount == 0)
	{
		return;
	}
	else if (MaxCount < 0)
	{
		if (ToInventory != FromInventory && ToInfoItem != nullptr)
		{
			ToSlotItem.bUnequipProccess = true;
			ToSlotItem.bUnequipAndStack = true;

			FromSlotItem.Count += ToSlotItem.Count;
			FromSlotItem.ID = ToSlotItem.ID;
			FromInfoItem = ToInfoItem;

			ToSlotItem.Count = 0;
		}
		else
		{
			MaxCount = 1;
		}
	}
	else if (MaxCount > 0)
	{
		int32 ItemsToAdd = FMath::Min(MaxCount, FromSlotItem.Count);
		ToSlotItem.Count = FromSlotItem.Count - ItemsToAdd;
		ToSlotItem.ID = FromSlotItem.ID;
		ToInfoItem = FromInfoItem;

		FromSlotItem.Count = ItemsToAdd;
	}

	FromInventory->SetItem(DraggedFrom->IndexInInventory, ToSlotItem);
	ToInventory->SetItem(DroppedTo->IndexInInventory, FromSlotItem);

	DraggedFrom->Clear();
	if (ToInfoItem)
	{
		DraggedFrom->AddItem(ToSlotItem, *ToInfoItem, ToSlotItem.GetIconTexture(InventoryItemsData, ToSlotItem.ID));
	}

	DroppedTo->Clear();
	DroppedTo->AddItem(FromSlotItem, *FromInfoItem, FromSlotItem.GetIconTexture(InventoryItemsData, FromSlotItem.ID));

}
