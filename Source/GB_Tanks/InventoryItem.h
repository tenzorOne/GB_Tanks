#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include <Engine/DataTable.h>
#include "InventoryItem.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Miscellaneous	UMETA(DisplayName = "Miscellaneous"),
	IT_Currency			UMETA(DisplayName = "Currency"),
	IT_Equipment		UMETA(DisplayName = "Equipment"),
	IT_Consumable		UMETA(DisplayName = "Consumable")

};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	IR_None				UMETA(DisplayName = "Unknown"),
	IR_Common			UMETA(DisplayName = "Common"),
	IR_Uncommon			UMETA(DisplayName = "Uncommon"),
	IR_Rare				UMETA(DisplayName = "Rare"),
	IR_Epic				UMETA(DisplayName = "Epic"),
	IR_Legendary		UMETA(DisplayName = "Legendary")

};

USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
	
	GENERATED_BODY()

	FInventoryItemInfo()
	: ID(), Name(), Description(), Type(EItemType::IT_Miscellaneous), Rarity(EItemRarity::IR_Common), Icon(), Mesh(), Damage(0), Armor(0), Intelligence(0)
	{}

public:
	UPROPERTY(EditAnywhere, Category = "General")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "General")
	FText Name;
	UPROPERTY(EditAnywhere, Category = "General")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typing")
	EItemType Type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typing")
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, Category = "Visual")
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, Category = "Visual")
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Damage;
	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Armor;
	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Intelligence;

};

USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
	
	GENERATED_BODY()

public:
	UTexture2D* GetIconTexture(UDataTable* DataTable, FName IconID) { SetItemIcon(DataTable, IconID); if (IconTexture) return IconTexture.Get(); else return nullptr; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FName ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	int32 Count;

private:
	void SetItemIcon(UDataTable* InDataTable, FName InItemID);
	
	UPROPERTY()
	TSoftObjectPtr<UTexture2D> IconTexture;

};