
#include "InventoryItem.h"

void FInventorySlotInfo::SetItemIcon(UDataTable* InDataTable, FName InItemID)
{
	if (InItemID == InDataTable->FindRow<FInventoryItemInfo>(InItemID, "")->ID)
	{
		IconTexture = InDataTable->FindRow<FInventoryItemInfo>(InItemID, "")->Icon;
	}

}
