// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioButtons.h"
#include "StyleSet.h"

void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	MyRadioButtons.Reset();

	Super::ReleaseSlateResources(bReleaseChildren);

}

TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
	// дл€ RadioButtons применен стиль WS_RadioButtons
	// при изменении цветовой схемы главного меню этот стиль не мен€етс€, то есть наши радио-баттоны независимы от всего остального
	this->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FRadioButtonsStyle>(FName("WS_RadioButtons"));
	
	MyRadioButtons = SNew(SRadioButtons)
	.Style(&WidgetStyle)
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
