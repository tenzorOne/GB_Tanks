// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SMiniMap.h"
#include "MiniMap.generated.h"

class SMiniMap;

/**
 * 
 */
UCLASS()
class GB_TANKS_API UMiniMap : public UWidget
{
	GENERATED_BODY()

public:
	void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	TSharedRef<SWidget> RebuildWidget() override;
	
protected:
	TSharedPtr<SMiniMap> MyMiniMap;

};
