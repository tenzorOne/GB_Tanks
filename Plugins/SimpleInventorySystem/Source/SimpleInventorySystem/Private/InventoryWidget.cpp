// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryDragDropOperation.h"
#include "InventoryComponent.h"
#include <Components/UniformGridPanel.h>


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UInventoryCellWidget* CellWidget;

	for (int32 i = 0; i < CellWidgets.Num(); i++)
	{
		CellWidget = CellWidgets[i];
		InitCellWidget(CellWidget);
		CellWidget->IndexInInventory = i;
		CellWidget->SetOwner(this);
		CellWidget->CountTextVisibilityChanged_Hardcode();
	}

}

void UInventoryWidget::Init(int32 ItemsNum)
{
	if (ItemCellsGrid)
	{
		ItemCellsGrid->ClearChildren();
		for (int32 i = 0; i < ItemsNum; i++)
		{
			UInventoryCellWidget* Widget = CreateCellWidget();
			Widget->IndexInInventory = i;
			Widget->SetOwner(this);
			ItemCellsGrid->AddChildToUniformGrid(Widget, i / ItemsInRow, i % ItemsInRow);
		}
	}

}

bool UInventoryWidget::AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, UTexture2D* InIconTexture, int32 SlotPosition /*= -1*/)
{
	if (ItemCellsGrid)
	{
		UInventoryCellWidget* WidgetToAddItem = nullptr;
		UInventoryCellWidget** WidgetToAddItemPtr = CellWidgets.FindByPredicate
		(
				[SlotPosition](UInventoryCellWidget* Widget)
				{
					return Widget && Widget->IndexInInventory == SlotPosition;
				}
		);

		if (WidgetToAddItemPtr)
		{
			WidgetToAddItem = *WidgetToAddItemPtr;
		}
		else
		{
			for (UInventoryCellWidget* CellWidget : CellWidgets)
			{
				if (!CellWidget->HasItem())
				{
					WidgetToAddItem = CellWidget;
					break;
				}
			}
		}

		if (WidgetToAddItem)
		{
			return WidgetToAddItem->AddItem(Item, ItemInfo, InIconTexture);
		}
	}
	
	return false;

}

UInventoryCellWidget* UInventoryWidget::CreateCellWidget()
{
	if (CellWidgetClass)
	{
		UInventoryCellWidget* CellWidget = CreateWidget<UInventoryCellWidget>(this, CellWidgetClass);
		CellWidgets.Add(CellWidget);

		InitCellWidget(CellWidget);
		
		return CellWidget;
	}
	
	return nullptr;

}

void UInventoryWidget::InitCellWidget(UInventoryCellWidget* Widget)
{
	if (Widget)
	{
		Widget->OnItemDrop.AddUObject(this, &UInventoryWidget::OnItemDropped);
		Widget->ParentInventoryWidget = this;
	}

}

void UInventoryWidget::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{	
	if (OnItemDrop.IsBound())
	{
		OnItemDrop.Broadcast(DraggedFrom, DroppedTo);
	}

}
