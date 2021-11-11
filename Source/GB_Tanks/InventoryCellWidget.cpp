// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCellWidget.h"
#include <Kismet/KismetMaterialLibrary.h>
#include <Materials/MaterialInstanceDynamic.h>
#include "InventoryManagerComponent.h"
#include "InventoryWidget.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "InventoryDragDropOperation.h"
#include <Components/OverlaySlot.h>
#include <Components/BorderSlot.h>
#include <Engine/Texture.h>
#include <Materials/MaterialLayersFunctions.h>


void UInventoryCellWidget::InitializeVisualProperties(const FInventoryItemInfo& ItemInfo)
{	
	if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(CellImage->Slot))
	{
		OverlaySlot->SetHorizontalAlignment(HAlign_Center);
		OverlaySlot->SetVerticalAlignment(VAlign_Center);
		CellImage->Brush.SetImageSize(FVector2D(256.f));

		if (UBorderSlot* BorderSlot = Cast<UBorderSlot>(TextOverlay->Slot))
		{
			BorderSlot->SetPadding(FMargin(FVector4(4.f)));
			BorderSlot->SetHorizontalAlignment(HAlign_Fill);
			BorderSlot->SetVerticalAlignment(VAlign_Fill);
		}

		OverlaySlot = Cast<UOverlaySlot>(CountText->Slot);
		OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(VAlign_Fill);
		CountText->SetJustification(ETextJustify::Right);
	}

	if (CellImage && CellMaterial && CellMaterialParameters)
	{
		CellMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, CellMaterial, FName(), EMIDCreationFlags::Transient);

		if (CellMaterialInstance)
		{
			if (ItemInfo.Icon)
			{
				CellMaterialInstance->SetTextureParameterValue(FName("ItemIcon"), ItemInfo.Icon.Get());
			}
			else
			{
				CellMaterialInstance->SetTextureParameterValue(FName("ItemIcon"), EmptyCellTexture);
			}
			
			if (bHasItem)
			{
				CellMaterialInstance->SetScalarParameterValue("EmptySlot", 0.f);
				if (bCountTextVisibleHardCode)
				{
					CountText->SetVisibility(ESlateVisibility::Visible);
				}
			}
			else
			{
				CellMaterialInstance->SetScalarParameterValue("EmptySlot", 1.f);
				CountText->SetVisibility(ESlateVisibility::Hidden);
			}

			UActorComponent* InventoryManagerPTR = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UInventoryManagerComponent::StaticClass());
			if (InventoryManagerPTR)
			{
				if (UInventoryManagerComponent* InventoryManagerComp = Cast<UInventoryManagerComponent>(InventoryManagerPTR))
				{
					EItemRarity ItemRarity = InventoryManagerComp->GetItemRarity(GetItem().ID);
					FLinearColor Color = FLinearColor(FColor(0.f));
					switch (ItemRarity)
					{
					case EItemRarity::IR_None: break;
					case EItemRarity::IR_Common:
						Color = UKismetMaterialLibrary::GetVectorParameterValue(this, CellMaterialParameters, "Common");
						CellMaterialInstance->SetVectorParameterValue("RarityColor", Color);
						break;
					case EItemRarity::IR_Uncommon:
						Color = UKismetMaterialLibrary::GetVectorParameterValue(this, CellMaterialParameters, "Uncommon");
						CellMaterialInstance->SetVectorParameterValue("RarityColor", Color);
						break;
					case EItemRarity::IR_Rare:
						Color = UKismetMaterialLibrary::GetVectorParameterValue(this, CellMaterialParameters, "Rare");
						CellMaterialInstance->SetVectorParameterValue("RarityColor", Color);
						break;
					case EItemRarity::IR_Epic:
						Color = UKismetMaterialLibrary::GetVectorParameterValue(this, CellMaterialParameters, "Epic");
						CellMaterialInstance->SetVectorParameterValue("RarityColor", Color);
						break;
					case EItemRarity::IR_Legendary:
						Color = UKismetMaterialLibrary::GetVectorParameterValue(this, CellMaterialParameters, "Legendary");
						CellMaterialInstance->SetVectorParameterValue("RarityColor", Color);
						break;
					}

					CellImage->SetBrushFromMaterial(CellMaterialInstance);

				}

			}

		}
	}
	
}


void UInventoryCellWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!bCountTextVisibleHardCode)
	{
		CountText->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UInventoryCellWidget::CountTextVisibilityChanged_Hardcode()
{
	CountText->SetVisibility(ESlateVisibility::Collapsed);

}

bool UInventoryCellWidget::HasItem()
{
	return bHasItem;

}

bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, UTexture2D* InIconTexture)
{
	if (bHasItem)
	{
		return false;
	}

	if (Item.Count <= 0)
	{
		Clear();
		return true;
	}

	if (CellImage)
	{
		CellImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(Item.Count)));
	}

	bHasItem = true;
	StoredItem = Item;
	SetIconTexture(InIconTexture);

	InitializeVisualProperties(ItemInfo);

	return true;

}

const FInventorySlotInfo& UInventoryCellWidget::GetItem()
{
	return StoredItem;

}

void UInventoryCellWidget::Clear()
{
	if (CellImage)
	{
		CellImage->SetBrushFromMaterial(CellMaterial);
	}
	if (CountText)
	{
		CountText->SetText(FText::FromString("0"));
	}

	bHasItem = false;
	StoredItem = FInventorySlotInfo();

}

void UInventoryCellWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bHasItem)
	{
		if (CellMaterialInstance)
		{
			CellMaterialInstance->SetScalarParameterValue("Highlight", 2.5f);
		}
	}
	else
	{
		HighlightCellImage->SetVisibility(ESlateVisibility::Visible);
	}

}

void UInventoryCellWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (bHasItem)
	{
		if (CellMaterialInstance)
		{
			CellMaterialInstance->SetScalarParameterValue("Highlight", 1.f);
		}
	}
	else
	{
		HighlightCellImage->SetVisibility(ESlateVisibility::Collapsed);
	}

}

FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bHasItem && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{		
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return FReply::Handled();

}

void UInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{	
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryDragDropOperation::StaticClass());
	if (OutOperation)
	{	 
		TSubclassOf<UInventoryCellWidget> CellWidgetClass;
		if (UInventoryWidget* ParentWidget = Cast<UInventoryWidget>(Owner))
		{
			CellWidgetClass = ParentWidget->GetCellWidgetClass();
		}

		UInventoryDragDropOperation* InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(OutOperation);
		InventoryDragDropOperation->Pivot = EDragPivot::CenterCenter;
		InventoryDragDropOperation->SourceCell = this;
		InventoryDragDropOperation->SourceCell->bHasItem ? true : false;
		if (InventoryDragDropOperation->SourceCell)
		{
			InventoryDragDropOperation->SourceCell->CellImage->SetBrushFromMaterial(DraggableCellMaterial);
			InventoryDragDropOperation->SourceCell->CountText->SetVisibility(ESlateVisibility::Collapsed);
		}

		UInventoryCellWidget* DraggableWidget = Cast<UInventoryCellWidget>(CreateWidget(GetWorld(), CellWidgetClass));
		InventoryDragDropOperation->DefaultDragVisual = DraggableWidget;
		DraggableWidget->bDraggableNow = true;
		if (DraggableWidget->bDraggableNow)
		{
			if (UMaterialInstanceDynamic* ItemMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DraggableItemMaterial, FName(), EMIDCreationFlags::Transient))
			{
				ItemMaterialInstance->SetTextureParameterValue(FName("ItemIcon"), InventoryDragDropOperation->SourceCell->IconTexture);
				DraggableWidget->CellImage->SetBrushFromMaterial(ItemMaterialInstance);
			}
			DraggableWidget->CountText->SetVisibility(ESlateVisibility::Visible);
			DraggableWidget->CountText->SetText(this->CountText->GetText());
		}
	}
	else
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}

}

void UInventoryCellWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(InOperation);

	if (InventoryDragDropOperation->SourceCell->bHasItem)
	{
		CellMaterialInstance->SetTextureParameterValue(FName("ItemIcon"), IconTexture);
		
		InventoryDragDropOperation->SourceCell->CellImage->SetBrushFromMaterial(CellMaterialInstance);
		InventoryDragDropOperation->SourceCell->CountText->SetVisibility(ESlateVisibility::Visible);
		InventoryDragDropOperation->SourceCell->CountText->SetText(this->CountText->GetText());
		this->bDraggableNow = false;
		InventoryDragDropOperation->SourceCell->bHasItem = true;
	}

}

bool UInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{	
	UInventoryDragDropOperation* InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(InOperation);

	if (CellMaterialInstance)
	{
		CellMaterialInstance->SetTextureParameterValue(FName("ItemIcon"), EmptyCellTexture);
		InventoryDragDropOperation->SourceCell->CellImage->SetBrushFromMaterial(CellMaterialInstance);
	}
	InventoryDragDropOperation->SourceCell->bDraggableNow = false;

	if (InventoryDragDropOperation)
	{	
		if (OnItemDrop.IsBound())
		{
			OnItemDrop.Broadcast(Cast<UInventoryCellWidget>(InventoryDragDropOperation->SourceCell), this);
		}
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

}
