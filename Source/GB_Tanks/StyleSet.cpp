
#include "StyleSet.h"
#include "Engine.h"
#include "SlateBasics.h"
#include "Slate/SlateGameResources.h"

TSharedPtr<ISlateStyle> FStyleSet::StylePtr = NULL;

TSharedPtr<ISlateStyle> FindStyle(const FString& ScopeToDirectory)
{
	TSharedPtr<FSlateGameResources> Style = FSlateGameResources::New(FName("StyleSet"), ScopeToDirectory, ScopeToDirectory);

	Style->Set("RedScheme", FLinearColor::Red);
	Style->Set("DesaturatedRed", FLinearColor(0.435f, 0.079f, 0.079f, 1.f));
	Style->Set("OpacityDarkRed", FLinearColor(0.078f, 0.045f, 0.045f, 0.75f));
	Style->Set("GreenScheme", FLinearColor::Green);
	Style->Set("DesaturatedGreen", FLinearColor(0.1345f, 0.269f, 0.156f, 1.f));
	Style->Set("OpacityDarkGreen", FLinearColor(0.045f, 0.078f, 0.045f, 0.75f));
	Style->Set("BlueScheme", FLinearColor::Blue);
	Style->Set("DesaturatedBlue", FLinearColor(0.134f, 0.134f, 0.358f, 1.f));
	Style->Set("OpacityDarkBlue", FLinearColor(0.045f, 0.045f, 0.078f, 0.75f));

	return Style;

}

void FStyleSet::Initialize()
{
	Shutdown();

	StylePtr = FindStyle("/Game/Tanks/UI");
	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);

}

void FStyleSet::Shutdown()
{
	if (StylePtr.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		ensure(StylePtr.IsUnique());
		StylePtr.Reset();
	}

}

const ISlateStyle& FStyleSet::Get()
{
	if (!StylePtr.IsValid())
	{
		Initialize();
	}
	
	return *StylePtr;

}
