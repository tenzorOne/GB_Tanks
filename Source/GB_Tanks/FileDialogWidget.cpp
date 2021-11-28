// Fill out your copyright notice in the Description page of Project Settings.


#include "FileDialogWidget.h"
#include "DesktopPlatformModule.h"
#include "FileMediaSource.h"
#include "IDesktopPlatform.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSoundComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Rendering/Texture2DResource.h"

static FTimerHandle TimerHandle;

void UFileDialogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BrowseButton && BrowseButtonText && ContentImage)
	{
		BrowseButton->OnReleased.AddDynamic(this, &ThisClass::OnButtonReleased);
	}

	// Так как логика происходит в виджете, то привязать ActorComponent, чем и является MediaSoundComponent, к виджету не получится,
	// поэтому я создаю актора, который ровным счетом ничего не делает, но позволяет компоненту прикрепиться к нему, а значит и функционировать.
	if (MediaPlayer)
	{
		AActor* ParentActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FActorSpawnParameters());
		ParentActor->SetActorLocation(FVector(0.0f));

		if (ParentActor)
		{
			UMediaSoundComponent* MediaSoundComponent = NewObject<UMediaSoundComponent>(ParentActor);
			MediaSoundComponent->SetMediaPlayer(MediaPlayer);
			MediaSoundComponent->RegisterComponent();
		}
	}
	
}

void UFileDialogWidget::SetMediaImageSize() const
{
	if (MediaPlayer->IsReady()) // когда плеер готов проиграть трек...
	{
		MediaPlayer->Play(); // ...мы его проигрываем...
		ContentImage->Brush.SetImageSize(FVector2D(MediaTexture->GetWidth(), MediaTexture->GetHeight())); // ...и устанавливаем размер виджета, который равен размеру медиа-текстуры
	}
	
	if (ContentImage->Brush.GetImageSize() != FVector2D(2.f)) // как только размер картинки виджета НЕ равен 2px (размер по умолчанию для медиа-текстуры), очищаем таймер, выходим из рекурсии
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
}

void UFileDialogWidget::OnButtonReleased()
{
	if (MediaPlayer->IsPlaying())
	{
		MediaPlayer->Pause(); // каждый раз, как открыеваем окно выбора файла, ставим текущий трек на паузу, если тот проигрывается
	}

	FString Path;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TArray<FString> OutFileNames;

		DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			"Choose content", "", "", FileTypes, 0, OutFileNames);

		if (OutFileNames.Num() > 0)
		{
			Path = OutFileNames[0];
		}
	}

	if (MediaPlayer && MediaMaterial && MediaTexture && !Path.IsEmpty())
	{
		MediaPlayer->Close(); // если все проверки пройдены, а главное, путь до файла существует, то закроем текущий трек

		UFileMediaSource* MediaSource = NewObject<UFileMediaSource>();
		MediaSource->FilePath = Path;

		if (MediaSource->FilePath.Contains(".MP4")) // если в пути файла найден файл с расширением .MP4 (а только этот тип видео мы и принимаем в данной реализации), то открываем файл...
		{
			ContentImage->SetBrushFromMaterial(MediaMaterial);
			MediaPlayer->OpenSource(MediaSource);

			/*
			 * Все инструкции, которые выполняются в следующих двух строках, направлены на поддержание корректного размера картинки Brush'а (куда выводим материал с медиа-текстурой),
			 * т.е. мы динамически меняем размер картинки виджета в методе SetImageSize() (см. выше).
			 * 
			 * Проблема с которой я столкнулся и причина по которой пришлось городить определенные костыли с рекурсией вызова метода через таймер, состоит в том, что при открытии
			 * медиа-ресурса, я ссылаюсь на размер (по ширине и высоте) медиа-текстуры. Но сразу после того, как медиа-плеер начинает проигрывание трека, размер этой самой текстуры
			 * остается равным 2px, хотя по факту он равен 1280х720 у того же видео "РикРолла". В дебаге же можно наблюдать, что движок не передает этот размер в интересующие меня переменные, когда
			 * как в игре можно заметить, что медиа-текстура обладает корректным размером (в PIE достаточно найти текстуру в контент-браузере и открыть её, чтобы увидеть размер).
			 *
			 * Я не стал слишком долго копаться в этом и предполагаю, что проблема кроется в тиках движка и то, как именно он обрабатывает только что открытый файл. Самой простой способ, которым я решил
			 * обойти этот нюанс - рекурсия. Я вызываю метод SetImageSize() и уже в нем очищу таймер и выйду из рекурсии, как только картинка в виджете примет корректный размер. Из-за этого подохода
			 * можно заметить микро задержку, когда я открываю новый видео-файл и виджет меняет свой размер под соотношение сторон (разрешение) этого видео.
			*/
			ContentImage->Brush.SetImageSize(FVector2D(2.f));
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SetMediaImageSize, 0.01f, true);
		}
		else // ...если файл не является видео-ресурсом, то можем утверждать, что открываем картинку
		{
			UTexture2D* ImportedMediaTexture = UKismetRenderingLibrary::ImportFileAsTexture2D(this, Path);
			if (ImportedMediaTexture)
			{
				ContentImage->SetBrushFromTexture(ImportedMediaTexture);
			}
			
			// в случае с простой картинкой, мне достаточно узнать размер импортированной текстуры и установить его в виджет
			ContentImage->Brush.SetImageSize(FVector2D(ImportedMediaTexture->Resource->GetSizeX(), ImportedMediaTexture->Resource->GetSizeY()));
		}
	}
	else // если пользователь не выбрал какой-либо файл или одна из проверок оказалась ложной, то просто продолжаем проигрывать текущий видео-ресурс
	{
		if (MediaPlayer->IsConnecting())
		{
			MediaPlayer->Play();
		}
	}
	
}
