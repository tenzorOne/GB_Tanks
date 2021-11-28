// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "FileDialogWidget.generated.h"

class UMediaPlayer;
class UMediaTexture;

/**
 * 
 */
UCLASS()
class GB_TANKS_API UFileDialogWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	const FString FileTypes =	"All Files(*.PNG;*.JPG;*.MP4)|*.PNG;*.JPG;*.MP4|"
								"Image Files(*.PNG;*.JPG;)|*.PNG;*.JPG;|"
								"Video Files(*.MP4)|*.MP4";
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetMediaImageSize() const;
	
	UFUNCTION()
	void OnButtonReleased();
	
protected:

	UPROPERTY(EditDefaultsOnly)
	UMediaPlayer* MediaPlayer;

	UPROPERTY(EditDefaultsOnly)
	UMediaTexture* MediaTexture;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* MediaMaterial;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BrowseButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BrowseButtonText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ContentImage;
	
};
