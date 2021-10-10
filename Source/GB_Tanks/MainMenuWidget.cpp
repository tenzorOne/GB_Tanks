// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include <TimerManager.h>
#include "Blueprint/WidgetTree.h"
#include "SMiniMap.h"

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

	if (CanvasHolder)
	{
		CanvasHolder->SetContent(SNew(SMiniMap));
	}

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