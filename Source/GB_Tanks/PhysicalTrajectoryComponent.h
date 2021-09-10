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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity Parameters")
		float Gravity = -9.8f;

public:	
	TArray<FVector> GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue = 0.f);

};
