// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationMarker.h"


// Add default functionality here for any ILocationMarker functions that are not pure virtual.
void ILocationMarker::NotifyOverlapWithActor(AActor* LocationMarker, AActor* OverlappedActor)
{
	if (OnLocationReached.IsBound())
	{
		OnLocationReached.Broadcast(LocationMarker, OverlappedActor);
	}
	
}
