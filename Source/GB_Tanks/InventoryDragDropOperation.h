
#pragma once

#include "CoreMinimal.h"
#include "InventoryCellWidget.h"
#include <Blueprint/DragDropOperation.h>
#include "InventoryDragDropOperation.generated.h"


/**
 *
 */
UCLASS()
class GB_TANKS_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryCellWidget* SourceCell;

};
