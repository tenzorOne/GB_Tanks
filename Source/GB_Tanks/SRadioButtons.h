// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "RadioButtonsWidgetStyle.h"

UENUM(BlueprintType)
enum class ERadioButtonChoice : uint8
{
	Radio0,
	Radio1,
	Radio2
};

DECLARE_DELEGATE_OneParam(FOnRadioButtonChanged, ERadioButtonChoice)

/**
 * 
 */
class GB_TANKS_API SRadioButtons : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadioButtons)
	{}

		SLATE_EVENT(FOnRadioButtonChanged, OnRadioButtonChanged)

		SLATE_STYLE_ARGUMENT(FRadioButtonsStyle, Style)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedRef<SWidget> CreateRadioButton(const FString& RadioText, ERadioButtonChoice RadioButtonChoice);
	ECheckBoxState IsRadioButtonChecked (ERadioButtonChoice RadioButtonID);
	void RadioButtonStateChanged(ECheckBoxState NewRadioState, ERadioButtonChoice RadioButtonID);
	void SetRadioButtonStyle(const FRadioButtonsStyle* InStyle);
	
protected:
	ERadioButtonChoice CurrentChoice;
	FOnRadioButtonChanged OnRadioButtonChanged;
	const FCheckBoxStyle* CheckBoxStyle;
	const FTextBlockStyle* TextStyle;

};
