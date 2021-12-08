// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"


// Add default functionality here for any IInteractableObject functions that are not pure virtual.


void IInteractableObject::NotifyInteractionFinished(AActor* InteractableObject, AActor* InteractFromActor)
{
	if (OnInteractionFinished.IsBound())
	{
		OnInteractionFinished.Broadcast(InteractableObject, InteractFromActor);
	}
	
}
