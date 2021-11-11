// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GB_TanksGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class GB_TANKS_API AGB_TanksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	AActor* GetActorByBoundTag(FName TagName);
	void UpdatePosition();
		
	AActor* ActorWithTag;

public:
	void BeginPlay() override;
	
	FVector2D GetActorPositionInBounds();
	static const FVector2D GetNormalizedPosition() { return NormalizedPosition; };

public:
	static FVector2D NormalizedPosition;
};
