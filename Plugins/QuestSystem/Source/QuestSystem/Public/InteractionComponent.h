// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionComponent.generated.h"

class UUserWidget;

UCLASS( ClassGroup=(Collision), meta=(DisplayName = "Interaction Collision", BlueprintSpawnableComponent) )
class QUESTSYSTEM_API UInteractionComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	UFUNCTION(BlueprintCallable)
	void Interact();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnInteractionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnInteractionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InteractMessageClass;
    
	UPROPERTY()
	UUserWidget* InteractMessageWidget;
	
	UPROPERTY()
	AActor* ActorToInteract;
	
};
