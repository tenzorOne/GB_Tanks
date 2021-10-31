
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
	if (DraggedFrom == nullptr || DroppedTo == nullptr || InventoryComponent == nullptr)
	{
		return;
	}

	InventoryComponent->SetItem(DraggedFrom->IndexInInventory, DroppedTo->GetItem());
	InventoryComponent->SetItem(DroppedTo->IndexInInventory, DraggedFrom->GetItem());

	FInventorySlotInfo FromSlotItem = DraggedFrom->GetItem();
	FInventorySlotInfo ToSlotItem = DroppedTo->GetItem();
	
	FInventoryItemInfo* FromInfoItem = GetItemData(FromSlotItem.ID);
	FInventoryItemInfo* ToInfoItem = GetItemData(ToSlotItem.ID);

	if (FromInfoItem == nullptr || (ToSlotItem.ID != NAME_None && ToInfoItem == nullptr))
	{
		return;
	}

	DraggedFrom->Clear();
	if (ToInfoItem)
	{
		DraggedFrom->AddItem(ToSlotItem, *ToInfoItem, ToSlotItem.GetIconTexture(InventoryItemsData, ToSlotItem.ID));
	}

	DroppedTo->Clear();
	DroppedTo->AddItem(FromSlotItem, *FromInfoItem, FromSlotItem.GetIconTexture(InventoryItemsData, FromSlotItem.ID));

}
