
#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Styling/ISlateStyle.h"

class GB_TANKS_API FStyleSet
{
public:
	static void Initialize();
	static void Shutdown();

	static const ISlateStyle& Get();

private:
	static TSharedPtr<class ISlateStyle> StylePtr;

};
