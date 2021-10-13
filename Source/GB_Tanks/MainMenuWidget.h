// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include <Components/HorizontalBox.h>
#include <Components/VerticalBox.h>
#include <Components/Image.h>
#include <Components/Overlay.h>
#include <Animation/WidgetAnimation.h>
#include <Components/NativeWidgetHost.h>
#include <Components/TextBlock.h>
#include "StyleSet.h"
#include "MainMenuWidget.generated.h"

UENUM()
enum class EButtonType : uint8 {
	BU_StartGame   UMETA(DisplayName = "Start Game"), 
	BU_Options	   UMETA(DisplayName = "Options"),       
	BU_Authors     UMETA(DisplayName = "Authors"),
	BU_Uncertain   UMETA(DisplayName = "Uncertain")
};

UENUM(BlueprintType)
enum class EColorScheme : uint8 {
	CO_BPDef		UMETA(DisplayName = "Blueprint Define"),
	CO_Red			UMETA(DisplayName = "Red"),
	CO_Green		UMETA(DisplayName = "Green"),
	CO_Blue			UMETA(DisplayName = "Blue")
};

/**
 * 
 */
UCLASS()
class GB_TANKS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	const float ANIMATION_PLAY_SPEED = 1.6f;

public:		
	void NativePreConstruct() override;
	void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnStartGameReleased();
	UFUNCTION()
	void OnOptionsReleased();
	UFUNCTION()
	void OnAuthorsReleased();
	UFUNCTION()
	void OnExitReleased();
	UFUNCTION()
	void OpenMenuAnimation(bool bAnimationForwardDirection, bool bRemoveBackground);
	UFUNCTION()
	void SubmenuChangeAnimation(bool bAnimationForwardDirection);

	bool FlipFlopSwitcher();
	void PlayAnimationByButtonType(EButtonType ButtonType);

	// шаблонная функция на изменение определенной цветовой компоненты у определенного элемента (виджета)
	template<typename T>
	void ChangeElementColorAndOpacity(T& Element, bool bOnlyTextOutlineChanges)
	{
		// если передаваемый в функцию виджет является UTextBlock, то мы будем обрабатывать изменения цвета текста и цвета его обводки
		if (UTextBlock* TempTextBlock = Cast<UTextBlock>(Element))
		{		
			FSlateFontInfo FontSettings = TempTextBlock->Font;
			
			// если хотим изменить только цвет обводки без изменения основного цвета текста
			if (bOnlyTextOutlineChanges)
			{
				switch (ColorScheme)
				{
				case EColorScheme::CO_BPDef:	break;

				case EColorScheme::CO_Red:		FontSettings.OutlineSettings.OutlineColor = FStyleSet::Get().GetColor("DesaturatedRed");
												TempTextBlock->SetFont(FontSettings);
												break;
					
				case EColorScheme::CO_Green:	FontSettings.OutlineSettings.OutlineColor = FStyleSet::Get().GetColor("DesaturatedGreen");
												TempTextBlock->SetFont(FontSettings);
												break;
					
				case EColorScheme::CO_Blue:		FontSettings.OutlineSettings.OutlineColor = FStyleSet::Get().GetColor("DesaturatedBlue");
												TempTextBlock->SetFont(FontSettings);
												break;
				}
			}
			// изменяем цвет текста и обводки
			else 
			{
				switch (ColorScheme)
				{
				case EColorScheme::CO_BPDef:	break;

				case EColorScheme::CO_Red:		TempTextBlock->SetColorAndOpacity(FStyleSet::Get().GetColor("RedScheme"));
												FontSettings.OutlineSettings.OutlineColor = FStyleSet::Get().GetColor("DesaturatedRed");
												TempTextBlock->SetFont(FontSettings);
												break;

				case EColorScheme::CO_Green:	TempTextBlock->SetColorAndOpacity(FStyleSet::Get().GetColor("GreenScheme"));
												FontSettings.OutlineSettings.OutlineColor = FStyleSet::Get().GetColor("DesaturatedGreen");
												TempTextBlock->SetFont(FontSettings);
												break;

				case EColorScheme::CO_Blue:		TempTextBlock->SetColorAndOpacity(FStyleSet::Get().GetColor("BlueScheme"));
												FontSettings.OutlineSettings.OutlineColor = FStyleSet::Get().GetColor("DesaturatedBlue");
												TempTextBlock->SetFont(FontSettings);
												break;
				}
			}

			Element = dynamic_cast<T>(TempTextBlock);

		}

		// если передаваемый в функцию виджет является UImage, то мы будем обрабатывать изменение цвета и прозрачности картинки
		if (UImage* TempImage = Cast<UImage>(Element))
		{
			switch (ColorScheme)
			{
			case EColorScheme::CO_BPDef:	break;

			case EColorScheme::CO_Red:		TempImage->SetColorAndOpacity(FStyleSet::Get().GetColor("OpacityDarkRed"));
											break;

			case EColorScheme::CO_Green:	TempImage->SetColorAndOpacity(FStyleSet::Get().GetColor("OpacityDarkGreen"));
											break;

			case EColorScheme::CO_Blue:		TempImage->SetColorAndOpacity(FStyleSet::Get().GetColor("OpacityDarkBlue"));
											break;
			}

			Element = dynamic_cast<T>(TempImage);

		}

	}

protected:
	UPROPERTY(EditAnywhere, Category = "Appearance")
		EColorScheme ColorScheme = EColorScheme::CO_Red;
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* MenuWidgetSwitcher;
	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* ButtonsGroup;
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* LevelButtonsGroup;
	UPROPERTY(meta = (BindWidget))
		UImage* Background;
	UPROPERTY(meta = (BindWidget))
		UButton* StartGameButton;
	UPROPERTY(meta = (BindWidget))
		UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
		UButton* AuthorsButton;
	UPROPERTY(meta = (BindWidget))
		UButton* ExitButton;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* BackgroundAnimation;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* SwitcherAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* AuthorsTextTranslation;

protected:
	EButtonType CurrentButtonType;
	EButtonType PreviousButtonType;
	bool bIsBackgroundValid = false;
	bool bFlipFlopSwitch = false;
	bool bDoOnce = true;

};
