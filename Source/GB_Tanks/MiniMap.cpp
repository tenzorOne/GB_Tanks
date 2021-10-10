// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"
#include "TimerManager.h"

void UMiniMap::ReleaseSlateResources(bool bReleaseChildren)
{
	MyMiniMap.Reset();
	
	Super::ReleaseSlateResources(bReleaseChildren);

}

TSharedRef<SWidget> UMiniMap::RebuildWidget()
{
	MyMiniMap = SNew(SMiniMap);

	return MyMiniMap.ToSharedRef();

}
