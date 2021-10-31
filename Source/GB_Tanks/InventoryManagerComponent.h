
#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include <Engine/DataTable.h>
#include "InventoryItem.h"
#include "InventoryWidget.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryComponent;
class UInventoryCellWidget;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GB_TANKS_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryManagerComponent();

 	UFUNCTION(BlueprintCallable)
	void Init(UInventoryComponent* InInventoryComponent);

	FInventoryItemInfo* GetItemData(FName ItemID);
	UTexture2D* GetItemIconTexture(UDataTable* DataTable, FName IconID);
	EItemRarity GetItemRarity(FName ItemID);

protected:
	void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);

protected:
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere)
	UDataTable* InventoryItemsData;
	UPROPERTY(BlueprintReadOnly)
	UInventoryWidget* InventoryWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere)
	int32 MinInventorySize = 20;

};
