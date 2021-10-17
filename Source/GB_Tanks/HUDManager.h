// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDManager.generated.h"

UENUM()
enum class EWidgetType : uint8
{
	None,
	MainMenu,
	GameOverMenu,
	PlayerHealthBar,
	TurretList
};

/**
 * 
 */
UCLASS()
class GB_TANKS_API AHUDManager : public AHUD
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "HUD Manager")
		bool bIsMenuOnly = false;
	UPROPERTY(EditAnywhere, Category = "HUD Manager")
		TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetClases;
	UPROPERTY()
		UUserWidget* CurrentWidget;

	EWidgetType CurrentWidgetType;
	FScriptDelegate OnActorDestroyedDelegate;

protected:
	UFUNCTION(BlueprintCallable, Category = "HUD Manager")
		UUserWidget* AddWidgetToViewport(EWidgetType WidgetTypeToUse, int32 ZOrder = 0);
	UFUNCTION(BlueprintCallable, Category = "HUD Manager")
		void RemoveWidgetFromViewport(UUserWidget* WidgetToRemove);
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD Manager")
		void DestroyingBindActor();

};
