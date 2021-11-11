// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioButtonsWidgetStyle.h"
#include "Styling/CoreStyle.h"

FRadioButtonsStyle::FRadioButtonsStyle()
{
	CheckBoxStyle = FCoreStyle::Get().GetWidgetStyle<FCheckBoxStyle>("Checkbox");
	TextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

}

FRadioButtonsStyle::~FRadioButtonsStyle()
{
}

const FName FRadioButtonsStyle::TypeName(TEXT("FRadioButtonsStyle"));

const FRadioButtonsStyle& FRadioButtonsStyle::GetDefault()
{
	static FRadioButtonsStyle Default;
	return Default;
}

void FRadioButtonsStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

