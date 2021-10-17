// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggableList.generated.h"

class UTextBlock;

/**
 * 
 * � ����� �� ��������� ��� ������ ������� �� �������������� �������� � ��� � ������� ������������ D&D �� ������� ��������� ������. ���� ���������� �������� � ����� ������ � � ������� ������� ���������� � ���������,
 * ���� �� � ������ �������� ������ � ��� ��� ������ ���� ��������� ��������� �������.
 * 
 * � ������ ������� (Turret.cpp), ����� ��� ������� (Cannon.cpp), � ������������ ������ ����� ������ � ������� (TowerMachineMaster.cpp), ������� �� ������ �� ��������������� �����, ���� ����������� ��������� ���������, ����� ������
 * ����� ��������� ���� �������� � ��������� ��� ������ ����� (���� �������� �� ������ ��������� �������� �� ������� �� ������ ������).
 * � �� ���� ������ ���������� ��� ��������� � ����, ��������� ���� ���� ��� UI � ������ ��, ����������, UI. ��� �� ����� ��� ����� �� �������� ������������ ������ ���������� �������.
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
	bool bDragActorToWorld = true;                // ����� ������� � ��� ��� ������ ������������ ������� ���� �� �����
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
