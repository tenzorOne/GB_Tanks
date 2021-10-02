// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AHUDManager::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (PlayerController)
		{
			if (APawn* PlayerPawn = PlayerController->GetPawn())
			{
				// ���� ����� ������ �������, �� ������� ������� �� "���������" ����������� �����, � ����� ������� �����, ������� ��������� � ��
				// ��� �������� ������� ����� ������, ��� WBP_GameOver
				OnActorDestroyedDelegate.BindUFunction(this, FName("DestroyingBindActor"));
				PlayerPawn->OnDestroyed.Add(OnActorDestroyedDelegate);
			}
			
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, EMouseLockMode::DoNotLock, false);
			PlayerController->SetShowMouseCursor(true);
		}
	}

}

// ����� ����������� ����� �� ��������� � UseWidget � ������� ����������

UUserWidget* AHUDManager::AddWidgetToViewport(EWidgetType WidgetTypeToUse, int32 ZOrder)
{
	if (CurrentWidgetType != WidgetTypeToUse)
	{
		if (TSubclassOf<UUserWidget> WidgetClassToUse = WidgetClases.FindRef(WidgetTypeToUse))
		{
			CurrentWidgetType = WidgetTypeToUse;
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClassToUse);

			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport(ZOrder);
				
				return CurrentWidget;
			}
		}
	}

	return nullptr;

}

// �������� ���������� �������

void AHUDManager::RemoveWidgetFromViewport(UUserWidget* WidgetToRemove)
{
	if (WidgetToRemove)
	{
		WidgetToRemove->RemoveFromParent();
		WidgetToRemove = nullptr;

		CurrentWidgetType = EWidgetType::None;
	}

}
