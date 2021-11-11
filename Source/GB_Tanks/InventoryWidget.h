// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.h"
#include "InventoryCellWidget.h"
#include <Components/UniformGridPanel.h>
#include "InventoryWidget.generated.h"

class UInventoryDragDropOperation;
class UInventoryComponent;


/**
 * 
 */
UCLASS()
class GB_TANKS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryComponent* RepresentedInventory;
	
	FOnItemDrop OnItemDrop;

public:
	virtual void NativeConstruct() override;

	void Init(int32 ItemsNum);
	void InitCellWidget(UInventoryCellWidget* Widget);
	bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, UTexture2D* InIconTexture, int32 SlotPosition = -1);
	TSubclassOf<UInventoryCellWidget> GetCellWidgetClass() { return CellWidgetClass; }

protected:
	UInventoryCellWidget* CreateCellWidget();
	void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UUniformGridPanel* ItemCellsGrid;
	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInRow = 5;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	TArray<UInventoryCellWidget*> CellWidgets;

};
