// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

class UObjective;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestStatusUpdated, AActor*); // сменить на AQuest?

UCLASS()
class QUESTSYSTEM_API AQuest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuest();

private:

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddInteractObjective();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddLocationObjective();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TakeQuest(AActor* Character);
	
	void OnObjectiveCompleted(UObjective* Objective);
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void UpdateLocation();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	FText Descrition;
	
	UPROPERTY(EditAnywhere)
	TArray<UObjective*> Objectives;

	UPROPERTY(EditAnywhere)
	AQuest* RequiredCompletedQuest;
	
	UPROPERTY(EditAnywhere)
	bool bIsStoryQuest = true;
	
	UPROPERTY(EditAnywhere)
	bool bKeepObjectivesOrder = true;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsTaken;

	FOnQuestStatusUpdated OnQuestStatusUpdated;
	
};
