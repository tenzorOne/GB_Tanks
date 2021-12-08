// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

class AInteractableActor;

// This class does not need to be modified.
UINTERFACE()
class UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished, AActor* /*InteractableObject*/, AActor* /*InteractFromActor*/);

/**
 * 
 */
class QUESTSYSTEM_API IInteractableObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor* InteractFromActor);
	UFUNCTION()
	virtual void Interact_Implementation(AActor* InteractFromActor) {}
	
	virtual void NotifyInteractionFinished(AActor* InteractableObject, AActor* InteractFromActor);
		
	FOnInteractionFinished OnInteractionFinished;
	
};

UCLASS()
class QUESTSYSTEM_API AInteractableActor : public AActor
{
	GENERATED_BODY()
    
	UFUNCTION(BlueprintCallable)
	void OnInteractionFinished(AActor* InteractFromActor)
	{
		if (IInteractableObject* InteractableObject = Cast<IInteractableObject>(this))
		{
			InteractableObject->NotifyInteractionFinished(this, InteractFromActor);
		}
	}
	
};