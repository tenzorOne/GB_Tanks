// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "InteractableObject.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnInteractionCompBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnInteractionCompEndOverlap);
	
}

void UInteractionComponent::Interact()
{
	if (ActorToInteract)
	{
		IInteractableObject::Execute_Interact(ActorToInteract, GetOwner());
	}
	
}

void UInteractionComponent::OnInteractionCompBeginOverlap(
		UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
		UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult & SweepResult)
{
	if (OtherActor->Implements<UInteractableObject>())
	{
		ActorToInteract = OtherActor;
		if (!InteractMessageWidget)
		{
			if (InteractMessageClass)
			{
				InteractMessageWidget = CreateWidget<UUserWidget>(GetWorld(), InteractMessageClass);
			}
		}

		if (InteractMessageWidget && !InteractMessageWidget->IsInViewport())
		{
			InteractMessageWidget->AddToViewport();
		}
	}
	
}

void UInteractionComponent::OnInteractionCompEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorToInteract = nullptr;
	if (InteractMessageWidget)
	{
		InteractMessageWidget->RemoveFromViewport();
	}
	
}
