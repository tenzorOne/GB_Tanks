// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Objective.generated.h"

class UObjective;

UENUM()
enum class EObjectiveType : uint8
{
	Interact,
	Location,
	Kill,
	Collect,
	Defend,
	Escort
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UObjective* /*Objective*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveFailed, UObjective* /*Objective*/);

UCLASS(Abstract)
class QUESTSYSTEM_API UObjective : public UObject
{
	GENERATED_BODY()

public:

	virtual void ActivateObjective(AActor* Character) {}

public:
	
	UPROPERTY(EditAnywhere)
	FText Descrition;
	
	UPROPERTY(VisibleAnywhere)
	EObjectiveType ObjectiveType;

	UPROPERTY(VisibleAnywhere)
	bool bIsCompleted;
	
	UPROPERTY(VisibleAnywhere)
	bool bCanBeCompleted;

	FOnObjectiveCompleted OnObjectiveCompleted;
	FOnObjectiveFailed OnObjectiveFailed;
	
};

UCLASS()
class QUESTSYSTEM_API UInteractionObjective : public UObjective
{
	GENERATED_BODY()

public:

	UInteractionObjective();
	
	void ActivateObjective(AActor* Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "InteractableObject"))
	AActor* Target;
	
};

UCLASS()
class QUESTSYSTEM_API ULocationObjective : public UObjective
{
	GENERATED_BODY()

public:

	ULocationObjective();

	void ActivateObjective(AActor* Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "LocationMarker"))
	AActor* Marker;
	
};

UCLASS()
class QUESTSYSTEM_API UDefendObjective : public UObjective
{
	GENERATED_BODY()

public:

	UDefendObjective();

	void ActivateObjective(AActor* Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "DefendContext"))
	AActor* DefendActor;
	
};