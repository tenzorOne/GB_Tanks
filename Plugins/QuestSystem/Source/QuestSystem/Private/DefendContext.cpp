// Fill out your copyright notice in the Description page of Project Settings.


#include "DefendContext.h"


// Add default functionality here for any IDefendContext functions that are not pure virtual.
void IDefendContext::NotifyDefendCompleted(AActor* DefenseStartedActor)
{
	if (OnCompletedDefend.IsBound())
	{
		OnCompletedDefend.Broadcast(DefenseStartedActor);
	}
	
}

void IDefendContext::NotifyDefendInterrupted(AActor* DefenseStartedActor, int32 PercentageBeforeCompleted)
{
	if (OnInterruptDefend.IsBound())
	{
		OnInterruptDefend.Broadcast(DefenseStartedActor, PercentageBeforeCompleted);
	}
	
}

void IDefendContext::NotifyDefendStarted()
{
	if (OnStartDefend.IsBound())
	{
		OnStartDefend.Broadcast();
	}
	
}


void IDefendContext::NotifyDefendFailed()
{
	if (OnFailedDefend.IsBound())
	{
		OnFailedDefend.Broadcast();
	}
	
}
