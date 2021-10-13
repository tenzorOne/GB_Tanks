// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include <TimerManager.h>
#include "Blueprint/WidgetTree.h"
#include "SMiniMap.h"

// ���������� ���-���������, ������ ��� � ����� � ������ ��������� ���� � ���������� � ��������� ������ � UMG/Slate
// ��, � ����, ��� ��� ������� ��� ������� ������ � ������ � �������� �� �����, ��� BindWidget
// ��, � ����, ��� ��� ������� ���� ����� (��� ��� ���� � RadioButtons) � ��������� ��� �� ���������� �������
// ����� � ������ ���������� ���� ������ (����� ��, ��� ��������):
//
//
// � ���� ���� ENum ColorScheme, ������� ������ ��������� �������������� �������� ���� (Red, Green, Blue - ��� ��� ��������� ��������� ������ ����� � ���� ��������� ������� ChangeElementColorAndOpacity() ����� FStyleSet::Get())
// ���� ENum � ���� ����������� � ���������, ��� ����� �� ��������� �� �������� ����� ���� ��������, ������� � ���-�� ���������� �����, � ���-����������
// ��� ����� ������� �� ��������� ������, range-based ������ � ��������, �� ���� ������ �������� �� ��� ������������:
// ��� ��� � �� ���������� ������� � ������, � ��������� ��������� ����� � �������� UMG-�������, ������� ���� � ������� � ��������� � �� ��� ��� ��������� ���� ��������� ������/��������� ������ ���� �� ������� �������� �����

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UTextBlock* StartTextBlock = Cast<UTextBlock>(StartGameButton->GetChildAt(0));
	ChangeElementColorAndOpacity(StartTextBlock, false);
	UTextBlock* OptionsTextBlock = Cast<UTextBlock>(OptionsButton->GetChildAt(0));
	ChangeElementColorAndOpacity(OptionsTextBlock, false);
	UTextBlock* AuthorsTextBlock = Cast<UTextBlock>(AuthorsButton->GetChildAt(0));
	ChangeElementColorAndOpacity(AuthorsTextBlock, false);
	UTextBlock* ExitTextBlock = Cast<UTextBlock>(ExitButton->GetChildAt(0));
	ChangeElementColorAndOpacity(ExitTextBlock, false);

	ChangeElementColorAndOpacity(Background, false);

	TArray<UWidget*> TempChildrenArray = LevelButtonsGroup->GetAllChildren();
	for (UWidget* CurrentChild : TempChildrenArray)
	{
		if (CurrentChild->GetClass() == UTextBlock::StaticClass())
		{
			CurrentChild = Cast<UTextBlock>(CurrentChild);
			ChangeElementColorAndOpacity(CurrentChild, true);
		}

		if (UButton* TempButton = Cast<UButton>(CurrentChild))
		{
			UTextBlock* TempTextBlock = Cast<UTextBlock>(TempButton->GetChildAt(0));
			ChangeElementColorAndOpacity(TempTextBlock, true);
		}
	}

	TempChildrenArray.Empty();

	if (UOverlay* SwitcherChild = Cast<UOverlay>(MenuWidgetSwitcher->GetChildAt(1)))
	{
		TempChildrenArray = SwitcherChild->GetAllChildren();
		for (UWidget* CurrentChild : TempChildrenArray)
		{
			if (UTextBlock* TempTextBlock = Cast<UTextBlock>(CurrentChild))
			{
				ChangeElementColorAndOpacity(TempTextBlock, true);
			}

			if (UVerticalBox* TempVerticalBox = Cast<UVerticalBox>(CurrentChild))
			{
				for (int i = 0; i < TempVerticalBox->GetChildrenCount(); ++i)
				{
					if (UHorizontalBox* TempHorizontalBox = Cast<UHorizontalBox>(TempVerticalBox->GetChildAt(i)))
					{
						if (UTextBlock* TempTextBlock = Cast<UTextBlock>(TempHorizontalBox->GetChildAt(0)))
						{
							ChangeElementColorAndOpacity(TempTextBlock, true);
						}
					}
				}
			}
		}
	}

	TempChildrenArray.Empty();

	if (UOverlay* SwitcherChild = Cast<UOverlay>(MenuWidgetSwitcher->GetChildAt(2)))
	{
		TempChildrenArray = SwitcherChild->GetAllChildren();
		for (int i = 0; i < TempChildrenArray.Num() + 1; ++i)
		{
			if (UVerticalBox* SubChild = Cast<UVerticalBox>(TempChildrenArray.Pop()))
			{
				for (int j = 0; j < SubChild->GetChildrenCount(); j += 3)
				{
					if (UTextBlock* TempTextBlock = Cast<UTextBlock>(SubChild->GetChildAt(j)))
					{
						ChangeElementColorAndOpacity(TempTextBlock, true);
					}
				}
			}
		}
	}

	TempChildrenArray.Empty();

}

// ����������� ������� ������ � ��������

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartGameButton)
	{
		StartGameButton->OnReleased.AddDynamic(this, &UMainMenuWidget::OnStartGameReleased);
	}
	if (OptionsButton)
	{
		OptionsButton->OnReleased.AddDynamic(this, &UMainMenuWidget::OnOptionsReleased);
	}
	if (AuthorsButton)
	{
		AuthorsButton->OnReleased.AddDynamic(this, &UMainMenuWidget::OnAuthorsReleased);
	}
	if (ExitButton)
	{
		ExitButton->OnReleased.AddDynamic(this, &UMainMenuWidget::OnExitReleased);
	}

	CurrentButtonType = PreviousButtonType = EButtonType::BU_Uncertain;

}

// �������� ���� � �� �� �������, �� � ������ ����� ������, ������ �� ����, ����� ������ ���� ������

void UMainMenuWidget::OnStartGameReleased()
{	
	PlayAnimationByButtonType(EButtonType::BU_StartGame);

}

void UMainMenuWidget::OnOptionsReleased()
{	
	PlayAnimationByButtonType(EButtonType::BU_Options);

}

void UMainMenuWidget::OnAuthorsReleased()
{
	PlayAnimationByButtonType(EButtonType::BU_Authors);

}

void UMainMenuWidget::OnExitReleased()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);

}

// �����-�������, ������� ������������, ��� ������������� ��� ����������� �������� �������� (������-�����)

bool UMainMenuWidget::FlipFlopSwitcher()
{
	return bFlipFlopSwitch = !bFlipFlopSwitch ? true : false;

}

// ��� ������� ������ �� �������� ���� �����

void UMainMenuWidget::PlayAnimationByButtonType(EButtonType ButtonType)
{
	// ����������� WidgetSwitcher � ����� ��������� (� ����. ������ �� ����-���) � ���������, ��� ������� ������� ������ �������� ������� ������������� ����
	MenuWidgetSwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
	CurrentButtonType = ButtonType;

	// ��������� ������, ��� ��� ������ ������� ��������� ������ �� �������� ���� �������� ���������� ������
	// ���� ���� ����� ������ ���� ������� ��� ������� ��������� �������
	if (bDoOnce)
	{
		bDoOnce = false;
		OpenMenuAnimation(FlipFlopSwitcher(), false);
		PreviousButtonType = ButtonType;
		
		return;
	}

	// ���� ���������� ������� ������ �� ��������� � �������, �� ������� ����� ����� ������� (����� ������������ ������������ � ����� �� �������, � �������)
	// � ��������� ������ ������� �����, ������� ������������� ��� ������� ��������, ������� ���� � �������� ������� ������
	if (PreviousButtonType != CurrentButtonType && bIsBackgroundValid)
	{
		SubmenuChangeAnimation(FlipFlopSwitcher());
	}
	else
	{
		OpenMenuAnimation(FlipFlopSwitcher(), true);
	}

	// ���������, ��� ������� ������� ������ �������� ������� ������������� ����
	PreviousButtonType = ButtonType;

}

// ����� �� �������� �������� � ��������� ��������

void UMainMenuWidget::OpenMenuAnimation(bool bAnimationForwardDirection, bool bRemoveBackground)
{	
//  	GEngine->AddOnScreenDebugMessage(-10, 2, FColor::Green, "MAIN: " + UEnum::GetDisplayValueAsText(CurrentButtonType).ToString());
//  	GEngine->AddOnScreenDebugMessage(-11, 2, FColor::Red, "MAIN: " + UEnum::GetDisplayValueAsText(PreviousButtonType).ToString());

	// ������ �� ����, ��� ����� � bAnimationForwardDirection ��������� �������� � ��������� ����������� ��� ����������
	if (bAnimationForwardDirection)
	{
		PlayAnimationForward(BackgroundAnimation, ANIMATION_PLAY_SPEED);
		bIsBackgroundValid = true;
	}
	else if (bRemoveBackground)
	{
		PlayAnimationReverse(BackgroundAnimation, ANIMATION_PLAY_SPEED);
		bIsBackgroundValid = false;
	}

	// ������ �� �������� ���� ������, ���������� ������ WidgetSwitcher'�, ����� �� ��������� ������ ������ ��������
	switch (CurrentButtonType)
	{
	case EButtonType::BU_StartGame:		MenuWidgetSwitcher->SetActiveWidgetIndex(0); break;
	case EButtonType::BU_Options:		MenuWidgetSwitcher->SetActiveWidgetIndex(1); break;
	case EButtonType::BU_Authors:		MenuWidgetSwitcher->SetActiveWidgetIndex(2); break;
	}

	// ������ �� ����, ��� ����� � bAnimationForwardDirection ��������� �������� � ��������� ����������� ��� ��������
	if (bAnimationForwardDirection)
	{
		PlayAnimationForward(SwitcherAnimation, ANIMATION_PLAY_SPEED);
	}
	else
	{
		PlayAnimationReverse(SwitcherAnimation, ANIMATION_PLAY_SPEED);
	}
	if (!UUserWidget::IsAnimationPlaying(AuthorsTextTranslation)) // ���. ���� ��� ������������ �������� ������ � ������� ������
	{
		PlayAnimation(AuthorsTextTranslation, 0.f, 0, EUMGSequencePlayMode::Forward, 0.2f, false);
	}

	MenuWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

// ����� �� ����� ������� (�� ����� � ������ ����, ��� ������)

void UMainMenuWidget::SubmenuChangeAnimation(bool bAnimationForwardDirection)
{	
// 	GEngine->AddOnScreenDebugMessage(-10, 2, FColor::Green, "SUBMENU: " + UEnum::GetDisplayValueAsText(CurrentButtonType).ToString());
// 	GEngine->AddOnScreenDebugMessage(-11, 2, FColor::Red, "SUBMENU: " + UEnum::GetDisplayValueAsText(PreviousButtonType).ToString());

	// ������ �� ����, � ����� ������ ��������� ������� �� ����� �������, ���������� ������ WidgetSwitcher'�, ����� �� ��������� ������ ������ ��������
	switch (PreviousButtonType)
	{
	case EButtonType::BU_StartGame:		MenuWidgetSwitcher->SetActiveWidgetIndex(0); break;
	case EButtonType::BU_Options:		MenuWidgetSwitcher->SetActiveWidgetIndex(1); break;
	case EButtonType::BU_Authors:		MenuWidgetSwitcher->SetActiveWidgetIndex(2); break;
	}	
	PlayAnimationReverse(SwitcherAnimation, ANIMATION_PLAY_SPEED); // ��������� �������� ��� �������� � �������� �����������

	// �������� ������� �� ������, ������� �������� ��������� �������� ��� ������ ������� � ������� �� ������ ��� ������ ����� SwitcherAnimation->GetEndTime / 2 (�� ���� ����� ���-�� ������� ���������� �������� ������� ������� / �� ���)
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
		{	
			switch (CurrentButtonType)
			{
			case EButtonType::BU_StartGame:		MenuWidgetSwitcher->SetActiveWidgetIndex(0); break;
			case EButtonType::BU_Options:		MenuWidgetSwitcher->SetActiveWidgetIndex(1); break;
			case EButtonType::BU_Authors:		MenuWidgetSwitcher->SetActiveWidgetIndex(2); break;
			}
			PlayAnimationForward(SwitcherAnimation, ANIMATION_PLAY_SPEED);
			MenuWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			bFlipFlopSwitch = true;
		});
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, SwitcherAnimation->GetEndTime() / 2.f, false);

}