// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoPlane.h"

#include "FileMediaSource.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "MediaSoundComponent.h"


// Sets default values
AVideoPlane::AVideoPlane()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVideoPlane::BeginPlay()
{
	Super::BeginPlay();

	FString Path;
	TArray<FString> OutFileNames;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		const FString FileTypes = 	"All Files(*.PNG;*.JPG;*.MP4)|*.PNG;*.JPG;*.MP4|"
									"Image Files(*.PNG;*.JPG;)|*.PNG;*.JPG;|"
									"Video Files(*.MP4)|*.MP4";
		constexpr uint32 SelectionFlag = 0;

		DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			"Choose content", "", "", FileTypes, SelectionFlag, OutFileNames);

		if (OutFileNames.Num() > 0)
		{
			Path = OutFileNames[0];
		}
	}
	
	if (MediaPlayer && !Path.IsEmpty())
	{
		UFileMediaSource* MediaSource = NewObject<UFileMediaSource>();
		MediaSource->FilePath = Path;
		MediaPlayer->OpenSource(MediaSource);

		UMediaSoundComponent* MediaSoundComponent = NewObject<UMediaSoundComponent>(this);
		MediaSoundComponent->SetMediaPlayer(MediaPlayer);
		MediaSoundComponent->RegisterComponent();
	}
	
}

// Called every frame
void AVideoPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

