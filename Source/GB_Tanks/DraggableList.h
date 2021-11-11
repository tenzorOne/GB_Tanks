// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggableList.generated.h"

class UTextBlock;

/**
 * 
 * я решил не создавать два разных виджета на перетаскивание объектов в мир и простую демонстрацию D&D на примере текстовых блоков. ¬есь функционал совмещен в одном классе и с помощью булевой переменной € определ€ю,
 * хочу ли € сейчас вытащить турель в мир или просто хочу потаскать текстовые виджеты.
 * 
 * ¬ логике “урелей (Turret.cpp), ѕушек дл€ турелей (Cannon.cpp), в родительском классе наших танков и турелей (TowerMachineMaster.cpp), который мы делали на соответствующем курсе, было произведено множество изменений, чтобы турель
 * могла таргетить себе подобных и вражеские дл€ игрока танки (сами турельки не совсем правильно работают по таргету на данный момент).
 * я не вижу смысла показывать все изменени€ в коде, поскольку этот курс про UI и делаем мы, собственно, UI. “ем не менее дл€ этого ƒ« пришлось переработать другие компоненты проекта.
 * 
 */
UCLASS()
class GB_TANKS_API UDraggableList : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void OnMouseButtonUp();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDragActorToWorld = true;                // т€нем объекты в мир или просто перет€гиваем виджеты друг на друга
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* NameText;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DraggedActorClass;
	UPROPERTY()
	AActor* DraggedActor;
	UPROPERTY()
	APlayerController* PlayerController;

public:
	UPROPERTY(EditAnywhere)
	FText TurretName;


};
