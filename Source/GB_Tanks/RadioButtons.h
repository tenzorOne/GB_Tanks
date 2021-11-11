// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SRadioButtons.h"
#include "RadioButtons.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonChangedEvent, ERadioButtonChoice, RadioButtonID);

/**
 * 
 */
UCLASS()
class GB_TANKS_API URadioButtons : public UWidget
{
	GENERATED_BODY()
	
public:
	void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(BlueprintAssignable, Category = "RadioButtons|Event")
	FOnRadioButtonChangedEvent OnRadioButtonChanged;
	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (DisplayName = "Style"))
	FRadioButtonsStyle WidgetStyle;

protected:
	TSharedRef<SWidget> RebuildWidget() override;
	void RadioButtonStateChanged(ERadioButtonChoice RadioButtonID);

protected:
	TSharedPtr<SRadioButtons> MyRadioButtons;

};
