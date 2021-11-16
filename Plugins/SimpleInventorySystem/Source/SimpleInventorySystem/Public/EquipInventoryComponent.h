// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "EquipInventoryComponent.generated.h"


/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class SIMPLEINVENTORYSYSTEM_API UEquipInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	UEquipInventoryComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item) override;
	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) override;
	void EquipItem(int32 SlotIndex, FName ItemID);
	void UnequipItem(int32 SlotIndex, FName ItemID);

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, EEquipSlot> EquipSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterial* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialParameterCollection* MaterialParameters;

	UPROPERTY()
	AActor* PlayerCharacter;
	
	FInventorySlotInfo* CurrentItem;

};
