// Fill out your copyright notice in the Description page of Project Settings.


#include "Objective.h"

#include "DefendContext.h"
#include "InteractableObject.h"
#include "LocationMarker.h"

UInteractionObjective::UInteractionObjective()
{
	ObjectiveType = EObjectiveType::Interact;
	
}

void UInteractionObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	if (IInteractableObject* InteractableTarget = Cast<IInteractableObject>(Target))
	{
		InteractableTarget->OnInteractionFinished.AddLambda([this, Character](AActor* InteractableObject, AActor* InteractFromActor)
		{
			if (bCanBeCompleted && Character == InteractFromActor)
			{
				bIsCompleted = true;
				if (OnObjectiveCompleted.IsBound())
				{
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}
	
}

ULocationObjective::ULocationObjective()
{
	ObjectiveType = EObjectiveType::Location;
	
}

void ULocationObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	if (ILocationMarker* LocationMarker = Cast<ILocationMarker>(Marker))
	{
		LocationMarker->OnLocationReached.AddLambda([this, Character](AActor* LocMarker, AActor* OverlappedActor)
		{
			if (bCanBeCompleted && Character == OverlappedActor)
			{
				bIsCompleted = true;
				if (OnObjectiveCompleted.IsBound())
				{
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});	
	}
	
}

UDefendObjective::UDefendObjective()
{
	ObjectiveType = EObjectiveType::Defend;
	
}

void UDefendObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	if (IDefendContext* DefendContext = Cast<IDefendContext>(DefendActor))
	{
		DefendContext->OnCompletedDefend.AddLambda([this, Character](AActor* DefenseStartedActor)
		{
			if (bCanBeCompleted && Character == DefenseStartedActor)
			{
				bIsCompleted = true;
				if (OnObjectiveCompleted.IsBound())
				{
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});

		DefendContext->OnFailedDefend.AddLambda([this, Character]()
		{
			bIsCompleted = false;
			if (OnObjectiveFailed.IsBound())
			{
				OnObjectiveFailed.Broadcast(this);
			}
		});
	}
	
}

