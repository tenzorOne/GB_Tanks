// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include <Components/HorizontalBox.h>
#include <Components/VerticalBox.h>
#include <Components/Image.h>
#include <Animation/WidgetAnimation.h>
#include "MainMenuWidget.generated.h"

UENUM()
enum class EButtonType : uint8 {
	BU_StartGame   UMETA(DisplayName = "Start Game"), 
	BU_Options	   UMETA(DisplayName = "Options"),       
	BU_Authors     UMETA(DisplayName = "Authors"),
	BU_Uncertain   UMETA(DisplayName = "Uncertain")
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

protected:
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
