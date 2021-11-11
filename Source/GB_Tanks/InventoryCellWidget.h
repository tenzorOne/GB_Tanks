// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "InventoryItem.h"
#include <Components/Overlay.h>
#include "InventoryCellWidget.generated.h"

class UInventoryWidget;
class UUniformGridPanel;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, UInventoryCellWidget* /*DraggedFrom*/, UInventoryCellWidget* /*DroppedTo*/);

/**
 * 
 */
UCLASS()
class GB_TANKS_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryWidget* ParentInventoryWidget;
	UPROPERTY(EditAnywhere)
	bool bCountTextVisibleHardCode = true;

	FOnItemDrop OnItemDrop;
	int32 IndexInInventory = -1;

public:
	virtual void NativePreConstruct() override;
	
	void CountTextVisibilityChanged_Hardcode();
	bool HasItem();
	bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, UTexture2D* InIconTexture);
	const FInventorySlotInfo& GetItem();
	void Clear();
	void SetOwner(UInventoryWidget* OwnerWidget) { Owner = OwnerWidget; }
	void SetIconTexture(UTexture2D* InTexture) { IconTexture = InTexture; }
	void InitializeVisualProperties(const FInventoryItemInfo& ItemInfo);

protected:
	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UOverlay* TextOverlay;
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* CellImage;
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* HighlightCellImage;
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CountText;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* EmptyCellTexture;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* CellMaterial;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DraggableItemMaterial;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DraggableCellMaterial;
	UPROPERTY(EditDefaultsOnly)
	UMaterialParameterCollection* CellMaterialParameters;
	
	bool bHasItem;
	bool bDraggableNow = false;
	UMaterialInstanceDynamic* CellMaterialInstance;
	FInventorySlotInfo StoredItem;
	UInventoryWidget* Owner;
	UTexture2D* IconTexture;

};
