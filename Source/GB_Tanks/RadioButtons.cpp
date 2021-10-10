// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioButtons.h"

void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	MyRadioButtons.Reset();

	Super::ReleaseSlateResources(bReleaseChildren);

}

TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
	MyRadioButtons = SNew(SRadioButtons)
	.OnRadioButtonChanged_UObject(this, &URadioButtons::RadioButtonStateChanged);
	
	return MyRadioButtons.ToSharedRef();

}

void URadioButtons::RadioButtonStateChanged(ERadioButtonChoice RadioButtonID)
{
	if (OnRadioButtonChanged.IsBound())
	{
		OnRadioButtonChanged.Broadcast(RadioButtonID);
	}

}
