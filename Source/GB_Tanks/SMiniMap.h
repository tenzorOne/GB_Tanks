// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCanvas.h"
#include "GB_TanksGameModeBase.h"

/**
 * 
 */
class GB_TANKS_API SMiniMap : public SCanvas
{
public:
	SLATE_BEGIN_ARGS(SMiniMap)
		: _PlayerPosition(AGB_TanksGameModeBase::NormalizedPosition)
	{}

	// ��� �� � �� ������� ������������ Attribute �/��� Argument, � ���� ��� ����� �� �������� ��������� ��� ��������. ������� � ���������� ����� RePaint (����������� � .cpp)
	// ����� ������� ��������/�������� ������ �����������? ���� ������ ����, ��� �������� � ��������� � ����, � ��� ���������� �� ��������, �� ��������, ��� ������ �������� (��� � �������), � �������� ��� � � �������� ����������� �����
	// � ����� �� ����� �� ��������, �� ������������... ��� ������ � SNew ����� ��������?

		//SLATE_ATTRIBUTE(FVector2D, PlayerPosition)
		SLATE_ARGUMENT(FVector2D, PlayerPosition)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

protected:
	EActiveTimerReturnType RePaint(double InCurrentTime, float InDeltaTime);

protected:
	FVector2D PlayerPos;

};
