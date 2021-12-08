// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LocationMarker.generated.h"

// This class does not need to be modified.
UINTERFACE()
class ULocationMarker : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocationReached, AActor* /*LocationMarker*/, AActor* /*OverlappedActor*/);

/**
 * 
 */
class QUESTSYSTEM_API ILocationMarker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void NotifyOverlapWithActor(AActor* LocationMarker, AActor* OverlappedActor);

	FOnLocationReached OnLocationReached;
	
};

UCLASS()
class QUESTSYSTEM_API ALocationMarkerActor : public AActor
{
	GENERATED_BODY()
    
	UFUNCTION(BlueprintCallable)
	void OnOverlapedByActor(AActor* OverlappingActor)
	{
		if (ILocationMarker* LocationMarker = Cast<ILocationMarker>(this))
		{
			LocationMarker->NotifyOverlapWithActor(this, OverlappingActor);
		}
	}
	
};
