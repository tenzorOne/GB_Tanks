// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipInventoryComponent.h"
#include <Kismet/KismetMaterialLibrary.h>
#include "InventoryManagerComponent.h"

UEquipInventoryComponent::UEquipInventoryComponent()
{
	EquipSlots.Add(0, EEquipSlot::EQ_CannonSlot);
	EquipSlots.Add(1, EEquipSlot::EQ_TrackSlot);
	EquipSlots.Add(2, EEquipSlot::EQ_ArmorSlot);
	EquipSlots.Add(3, EEquipSlot::EQ_TowerSlot);
	EquipSlots.Add(4, EEquipSlot::EQ_EngineSlot);

}

void UEquipInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// получаем указатель на игрока в начале игры
	PlayerCharacter = GetOwner();

}

void UEquipInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (DefaultMaterial)
	{
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_CannonSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_CannonSlot"), 0.f, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_ArmorSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_ArmorSlot"), 0.f, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_TowerSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_TowerSlot"), 0.f, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideVectorParameterDefault(FName("EQ_EngineSlot"), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
		DefaultMaterial->OverrideScalarParameterDefault(FName("EQ_EngineSlot"), 0.f, false, ERHIFeatureLevel::SM5);
	}
	
	Super::EndPlay(EndPlayReason);

}

/*
 *
 *	Никаких особых изменений в код я не вносил, лишь дописал модуль "UMG" в .cs плагина и удалил некоторые неиспользуемые поля в некоторых классах;
 *	Самое серьезное изменение произошло тут (EquipInventoryComponent), в методах SetItem и Equip/UnequipItem;
 *
 *	Т.к. именно этот компонент отвечает за процесс экипировки, я решил, минуя интерфейсы и прочие связи, получить указатель на игрока (см. BeginPlay()),
 *	и, если он существует, то вызываю методы Equip или Unequip, которые реализованы здесь же, в этом комопненте, вместо реализации их на конкретном персонаже.
 *	В самих методах процесс изменения материала при экипировке/снятии итема почти не поменялся, но больше я не назначаю материал на меши танка повторно,
 *	поскольку теперь просто беру указатель на уже существующий базовый материал танка, который мы должны указать в БП компонента и уже работаю с ним.
 *
 *	Ни один из методов не будет вызван, если указатель на игрока (овнера компонента, по сути) и/или указатель на материал будут нулевыми. Это гарантирует,
 *	что перенесенные методы из нашего игрока прямо в EquipInventoryComponent будут корректно работать и не крашнут игру;
 *	
*/
void UEquipInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{	
	if (Item.ID != NAME_None)
	{
		CurrentItem = const_cast<FInventorySlotInfo*>(&Item);
	}

	FInventorySlotInfo* EquippedItem = GetItem(SlotIndex);
	if (EquippedItem && CurrentItem->bUnequipProccess)
	{
		if (PlayerCharacter)
		{
			if (!CurrentItem->bUnequipAndStack)
			{
				CurrentItem->bUnequipProccess = false;
			}
			UnequipItem(SlotIndex, Item.ID);
		}
	}

	Super::SetItem(SlotIndex, Item);

	if (!CurrentItem->bUnequipProccess && Item.ID != NAME_None)
	{
		if (PlayerCharacter)
		{
			CurrentItem->bUnequipProccess = true;
			EquipItem(SlotIndex, Item.ID);
		}
	}

	CurrentItem->bUnequipAndStack = false;

}

int32 UEquipInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item)
{
	if (Item.Type != EItemType::IT_Equipment || !EquipSlots.Contains(SlotIndex) || EquipSlots.FindChecked(SlotIndex) != Item.EquipmentSlot)
	{
		return 0;
	}

	return 1;

}

void UEquipInventoryComponent::EquipItem(int32 SlotIndex, FName ItemID)
{
	if (DefaultMaterial)
	{
		if (UMaterialInstanceDynamic* MainMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DefaultMaterial, FName()))
		{
			TArray<UActorComponent*> ActorComponents;
			PlayerCharacter->GetComponents(UInventoryManagerComponent::StaticClass(), ActorComponents);
			if (ActorComponents.Num() != 0)
			{
				if (UInventoryManagerComponent* InventoryManagerComponent = Cast<UInventoryManagerComponent>(ActorComponents[0]))
				{
					EItemRarity ItemRarity = InventoryManagerComponent->GetItemRarity(ItemID);

					if (ItemRarity != EItemRarity::IR_None && MaterialParameters)
					{
						FName SlotName = StaticEnum<EEquipSlot>()->GetNameByValue(SlotIndex == 99 ? 0 : SlotIndex);
						FString StringSlotName = SlotName.ToString();
						StringSlotName = StringSlotName.RightChop(12);

						switch (ItemRarity)
						{
						case EItemRarity::IR_None:
							break;
						case EItemRarity::IR_Common:
							MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Common"));
							MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
							DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Common"), true, ERHIFeatureLevel::SM5);
							DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
							break;
						case EItemRarity::IR_Uncommon:
							MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Uncommon"));
							MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
							DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Uncommon"), true, ERHIFeatureLevel::SM5);
							DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
							break;
						case EItemRarity::IR_Rare:
							MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Rare"));
							MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
							DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Rare"), true, ERHIFeatureLevel::SM5);
							DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
							break;
						case EItemRarity::IR_Epic:
							MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Epic"));
							MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
							DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Epic"), true, ERHIFeatureLevel::SM5);
							DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
							break;
						case EItemRarity::IR_Legendary:
							MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Legendary"));
							MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 1.f);
							DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), UKismetMaterialLibrary::GetVectorParameterValue(this, MaterialParameters, "Legendary"), true, ERHIFeatureLevel::SM5);
							DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 1.f, true, ERHIFeatureLevel::SM5);
							break;
						}
					}
				}
			}
		}
	}

}

void UEquipInventoryComponent::UnequipItem(int32 SlotIndex, FName ItemID)
{
	FName SlotName = StaticEnum<EEquipSlot>()->GetNameByValue(SlotIndex == 99 ? 0 : SlotIndex);
	FString StringSlotName = SlotName.ToString();
	StringSlotName = StringSlotName.RightChop(12);

	if (DefaultMaterial)
	{
		if (UMaterialInstanceDynamic* MainMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DefaultMaterial, FName()))
		{
			MainMaterialInstance->SetVectorParameterValue(FName(StringSlotName), FLinearColor::Black);
			MainMaterialInstance->SetScalarParameterValue(FName(StringSlotName), 0.f);
			DefaultMaterial->OverrideVectorParameterDefault(FName(StringSlotName), FLinearColor::Black, false, ERHIFeatureLevel::SM5);
			DefaultMaterial->OverrideScalarParameterDefault(FName(StringSlotName), 0.f, false, ERHIFeatureLevel::SM5);
		}
	}

}
