// Fill out your copyright notice in the Description page of Project Settings.


#include "SRadioButtons.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRadioButtons::Construct(const FArguments& InArgs)
{
	OnRadioButtonChanged = InArgs._OnRadioButtonChanged;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			CreateRadioButton("Option 1", ERadioButtonChoice::Radio0)
		]

		+ SVerticalBox::Slot()
		[
			CreateRadioButton("Option 2", ERadioButtonChoice::Radio1)
		]

		+ SVerticalBox::Slot()
		[
			CreateRadioButton("Option 3", ERadioButtonChoice::Radio2)
		]
	];
	
}

TSharedRef<SWidget> SRadioButtons::CreateRadioButton(const FString& RadioText, ERadioButtonChoice RadioButtonChoice)
{
	return SNew(SCheckBox)
		   .IsChecked(MakeAttributeRaw(this, &SRadioButtons::IsRadioButtonChecked, RadioButtonChoice))
		   .OnCheckStateChanged(this, &SRadioButtons::RadioButtonStateChanged, RadioButtonChoice)
		   [
			   SNew(STextBlock)
			   .Text(FText::FromString(RadioText))
		   ];

}


void SRadioButtons::RadioButtonStateChanged(ECheckBoxState NewRadioState, ERadioButtonChoice RadioButtonID)
{
	if (NewRadioState == ECheckBoxState::Checked)
	{
		CurrentChoice = RadioButtonID;
		OnRadioButtonChanged.ExecuteIfBound(CurrentChoice);
	}

}

ECheckBoxState SRadioButtons::IsRadioButtonChecked(ERadioButtonChoice RadioButtonID)
{
	return (CurrentChoice == RadioButtonID) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
