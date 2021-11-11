// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicalTrajectoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GB_TANKS_API UPhysicalTrajectoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhysicalTrajectoryComponent();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Velocity Parameters")
		FVector OutVelocity;

protected:
	AActor* CurrentTarget;

public:		
	bool SuggestVeloctiy(FVector LocationToCalculate);
	TArray<FVector> GenerateTrajectory(float& SimStep, float& MaxSimTime, bool& ShowTrajectory);
	void SetCurrentTarget(AActor* InTarget) { CurrentTarget = InTarget; };
	AActor* GetCurrentTarget() { return CurrentTarget; };

};
