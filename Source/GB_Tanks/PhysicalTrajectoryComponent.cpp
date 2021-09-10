// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicalTrajectoryComponent.h"

// Sets default values for this component's properties
UPhysicalTrajectoryComponent::UPhysicalTrajectoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

TArray<FVector> UPhysicalTrajectoryComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue /*= 0.f*/)
{
	TArray<FVector> Trajectory;
	FVector GravityVector(0, 0, Gravity);

	for (float Time = 0; Time < MaxTime; Time = Time + TimeStep)
	{
		FVector Position = StartPos + Velocity * Time + GravityVector * Time * Time / 2;
		if (Position.Z <= MinZValue)
		{
			break;
		}

		Trajectory.Add(Position);
	}

	return Trajectory;

}
