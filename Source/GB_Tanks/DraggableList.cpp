// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggableList.h"
#include <Components/TextBlock.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Components/VerticalBox.h>
#include <Kismet/GameplayStatics.h>
#include "TankPlayerController.h"
#include <Components/VerticalBoxSlot.h>

void UDraggableList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (NameText)
	{
		NameText->SetText(TurretName);
	}

	// если мы не спавним турель по нажатию на виджет, то не отоброжаем тултип виджета
	if (!bDragActorToWorld)
	{
		this->SetToolTipText(FText());
	}

}

void UDraggableList::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (ATankPlayerController* PC = Cast<ATankPlayerController>(PlayerController))
	{
		PC->OnMouseButtonUp.AddUObject(this, &UDraggableList::OnMouseButtonUp);
	}

}

void UDraggableList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DraggedActor && PlayerController)
	{
		FVector WorldMouseLocation;
		FVector WorldMouseDirection;
		PlayerController->DeprojectMousePositionToWorld(WorldMouseLocation, WorldMouseDirection);

		FHitResult HitResult;
		float TraseDistance = 1000000.f;
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(DraggedActor);
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			WorldMouseLocation,
			WorldMouseLocation + WorldMouseDirection * TraseDistance,
			ECollisionChannel::ECC_Visibility,
			Param);

		if (HitResult.Actor.Get())
		{
			DraggedActor->SetActorLocation(HitResult.Location);
		}
	}

}

// здесь определяем как именно мы хотим взаимодействовать с виджетом, а потом выполняем соответствующий блок
FReply UDraggableList::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bDragActorToWorld)
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			DraggedActor = GetWorld()->SpawnActor<AActor>(DraggedActorClass);
		}
		
		return FReply::Handled();

	}
	else
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}

	return FReply::Unhandled();

}

void UDraggableList::OnMouseButtonUp()
{
	if (DraggedActor)
	{
		DraggedActor = nullptr;
	}

}

void UDraggableList::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(OutOperation->StaticClass());

	if (OutOperation)
	{
		UDragDropOperation* DragAndDrop = Cast<UDragDropOperation>(OutOperation);
		DragAndDrop->DefaultDragVisual = this;
		DragAndDrop->Pivot = EDragPivot::TopLeft; // определяем опорную точку для операции в левый верхний угол виджета

		SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.7f));
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}

}

void UDraggableList::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	SetVisibility(ESlateVisibility::Visible);

}

bool UDraggableList::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UVerticalBox* VerticalBox = Cast<UVerticalBox>(GetParent());
	UDragDropOperation* DragAndDrop = Cast<UDragDropOperation>(InOperation);
	if (DragAndDrop && VerticalBox && this != DragAndDrop->DefaultDragVisual)
	{
		int32 IndexDragged = VerticalBox->GetChildIndex(DragAndDrop->DefaultDragVisual);
		if (IndexDragged != -1)
		{
			VerticalBox->RemoveChildAt(IndexDragged);
			TArray<UWidget*> VBChildren = VerticalBox->GetAllChildren();
			VerticalBox->ClearChildren();

			UVerticalBoxSlot* VerticalBoxSlot;
			for (int32 Counter = 0; Counter < VBChildren.Num(); ++Counter)
			{
				// когда перетаскиваемый виджет будет находиться НАД позицией текущего виджета (на который навелись), то текущий виджет будет перенесен вверх по иерархии;
				// когда перетаскиваемый виджет будет находиться ПОД позицией текущего виджета, то текущий виджет будет перенесен вниз по иерархии
				if (DragAndDrop->DefaultDragVisual->GetTickSpaceGeometry().GetAbsolutePosition() < InGeometry.GetAbsolutePositionAtCoordinates(FVector2D(0.5f)))
				{
					// методы SetHorizontalAlignment и SetVerticalAlignment позволяют удерживать те же точки выравнивания перетаскиваемых виджетов, что были заданы до перестройки родительского виджета
					if (VBChildren[Counter] == this)
					{
						VerticalBox->AddChildToVerticalBox(DragAndDrop->DefaultDragVisual);
						VerticalBoxSlot = Cast<UVerticalBoxSlot>(DragAndDrop->DefaultDragVisual->Slot);
						VerticalBoxSlot->SetHorizontalAlignment(HAlign_Right);
						VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
					}
					VerticalBox->AddChildToVerticalBox(VBChildren[Counter]);
					VerticalBoxSlot = Cast<UVerticalBoxSlot>(VBChildren[Counter]->Slot);
					VerticalBoxSlot->SetHorizontalAlignment(HAlign_Right);
					VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
				}
				else
				{
					VerticalBox->AddChildToVerticalBox(VBChildren[Counter]);
					VerticalBoxSlot = Cast<UVerticalBoxSlot>(VBChildren[Counter]->Slot);
					VerticalBoxSlot->SetHorizontalAlignment(HAlign_Right);
					VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
					if (VBChildren[Counter] == this)
					{
						VerticalBox->AddChildToVerticalBox(DragAndDrop->DefaultDragVisual);
						VerticalBoxSlot = Cast<UVerticalBoxSlot>(DragAndDrop->DefaultDragVisual->Slot);
						VerticalBoxSlot->SetHorizontalAlignment(HAlign_Right);
						VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
					}
				}
			}
		}
	}
	
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

}

bool UDraggableList::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation && InOperation->DefaultDragVisual)
	{
		Cast<UUserWidget>(InOperation->DefaultDragVisual)->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		InOperation->DefaultDragVisual->SetVisibility(ESlateVisibility::Visible);
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

}
