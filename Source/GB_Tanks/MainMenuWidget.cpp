// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include <TimerManager.h>
#include "Blueprint/WidgetTree.h"
#include "SMiniMap.h"

// гигантский пре-констракт, потому что я хотел и дальше пробовать себя в синтаксисе и принципах работы с UMG/Slate
// да, я знаю, что мог создать все виджеты текста в хэдере и пометить их метой, как BindWidget
// да, я знаю, что мог создать один стиль (как это было с RadioButtons) и применить его на однотипные виджеты
// здесь я просто попробовал иной подход (опять же, для практики):
//
//
// У меня есть ENum ColorScheme, которое хранит несколько подготовленных цветовых схем (Red, Green, Blue - все они позволяют применять разную схему в моей шаблонной функции ChangeElementColorAndOpacity() через FStyleSet::Get())
// этот ENum я могу переключать в редакторе, что сразу же отразится на цветовой схеме всех виджетов, которые я как-то препарирую здесь, в пре-констракте
// сам метод состоит из множества кастов, range-based циклов и вложений, но суть проста несмотря на всю громоздкость:
// так как я не привязываю виджеты в Плюсах, я перебираю множество детей в иерархии UMG-виджета, которая есть у виджета в редакторе и по ней уже выставляю всем элементам текста/картинкам нужный цвет от текущей цветовой схемы

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

// привязываем нажатие кнопок к функциям

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

// вызываем одну и ту же функцию, но с разным типом кнопки, исходя из того, какая именно была нажата

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

// метод-тумблер, который используется, как переключатель для направления анимаций виджетов (вперед-назад)

bool UMainMenuWidget::FlipFlopSwitcher()
{
	return bFlipFlopSwitch = !bFlipFlopSwitch ? true : false;

}

// при нажатии кнопок мы вызываем этот метод

void UMainMenuWidget::PlayAnimationByButtonType(EButtonType ButtonType)
{
	// переключаем WidgetSwitcher в режим видимости (с откл. тестом на селф-хит) и указываем, что текущая нажатая кнопка является кнопкой определенного типа
	MenuWidgetSwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
	CurrentButtonType = ButtonType;

	// небольшой гарант, что при первом нажатии сработает логика на открытие всех виджетов конкретной кнопки
	// этот блок будет вызван лишь однажды для каждого подобного виджета
	if (bDoOnce)
	{
		bDoOnce = false;
		OpenMenuAnimation(FlipFlopSwitcher(), false);
		PreviousButtonType = ButtonType;
		
		return;
	}

	// если предыдущая нажатая кнопка НЕ совпадает с текущей, то вызовем метод смены подменю (когда пользователь переключился с Опций на Авторов, к примеру)
	// в противном случае вызовем метод, который воспроизведет все главные анимации, которые есть у виджетов нажатой кнопки
	if (PreviousButtonType != CurrentButtonType && bIsBackgroundValid)
	{
		SubmenuChangeAnimation(FlipFlopSwitcher());
	}
	else
	{
		OpenMenuAnimation(FlipFlopSwitcher(), true);
	}

	// указываем, что прошлая нажатая кнопка является кнопкой определенного типа
	PreviousButtonType = ButtonType;

}

// метод на основные анимации и раскрытие виджетов

void UMainMenuWidget::OpenMenuAnimation(bool bAnimationForwardDirection, bool bRemoveBackground)
{	
//  	GEngine->AddOnScreenDebugMessage(-10, 2, FColor::Green, "MAIN: " + UEnum::GetDisplayValueAsText(CurrentButtonType).ToString());
//  	GEngine->AddOnScreenDebugMessage(-11, 2, FColor::Red, "MAIN: " + UEnum::GetDisplayValueAsText(PreviousButtonType).ToString());

	// исходя из того, что будет в bAnimationForwardDirection проиграем анимацию в указанном направлении для бэкграунда
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

	// исходя из текущего типа кнопки, переключим индекс WidgetSwitcher'а, чтобы он отоброжал нужную группу виджетов
	switch (CurrentButtonType)
	{
	case EButtonType::BU_StartGame:		MenuWidgetSwitcher->SetActiveWidgetIndex(0); break;
	case EButtonType::BU_Options:		MenuWidgetSwitcher->SetActiveWidgetIndex(1); break;
	case EButtonType::BU_Authors:		MenuWidgetSwitcher->SetActiveWidgetIndex(2); break;
	}

	// исходя из того, что будет в bAnimationForwardDirection проиграем анимацию в указанном направлении для свитчера
	if (bAnimationForwardDirection)
	{
		PlayAnimationForward(SwitcherAnimation, ANIMATION_PLAY_SPEED);
	}
	else
	{
		PlayAnimationReverse(SwitcherAnimation, ANIMATION_PLAY_SPEED);
	}
	if (!UUserWidget::IsAnimationPlaying(AuthorsTextTranslation)) // доп. блок для проигрывания анимации текста в подменю Авторы
	{
		PlayAnimation(AuthorsTextTranslation, 0.f, 0, EUMGSequencePlayMode::Forward, 0.2f, false);
	}

	MenuWidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

// метод на смену подменю (из Опции в Начать игру, как пример)

void UMainMenuWidget::SubmenuChangeAnimation(bool bAnimationForwardDirection)
{	
// 	GEngine->AddOnScreenDebugMessage(-10, 2, FColor::Green, "SUBMENU: " + UEnum::GetDisplayValueAsText(CurrentButtonType).ToString());
// 	GEngine->AddOnScreenDebugMessage(-11, 2, FColor::Red, "SUBMENU: " + UEnum::GetDisplayValueAsText(PreviousButtonType).ToString());

	// исходя из того, с какой кнопки произошел переход на новое подменю, переключим индекс WidgetSwitcher'а, чтобы он отоброжал нужную группу виджетов
	switch (PreviousButtonType)
	{
	case EButtonType::BU_StartGame:		MenuWidgetSwitcher->SetActiveWidgetIndex(0); break;
	case EButtonType::BU_Options:		MenuWidgetSwitcher->SetActiveWidgetIndex(1); break;
	case EButtonType::BU_Authors:		MenuWidgetSwitcher->SetActiveWidgetIndex(2); break;
	}	
	PlayAnimationReverse(SwitcherAnimation, ANIMATION_PLAY_SPEED); // проиграем анимацию для свитчера в обратном направлении

	// забиндим делегат на лямбду, которая позволит проиграть анимацию уже нового подменю в которое мы только что попали через SwitcherAnimation->GetEndTime / 2 (то есть через кол-во времени предыдущей анимации СТАРОГО подменю / на два)
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