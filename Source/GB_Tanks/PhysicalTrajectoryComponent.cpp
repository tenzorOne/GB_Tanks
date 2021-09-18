// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicalTrajectoryComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UPhysicalTrajectoryComponent::UPhysicalTrajectoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UPhysicalTrajectoryComponent::SuggestVeloctiy()
{
	return UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutVelocity, GetOwner()->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

}

TArray<FVector> UPhysicalTrajectoryComponent::GenerateTrajectory(float& SimStep, float& MaxSimTime, bool& ShowTrajectory)
{
	FHitResult HitResult;
	TArray<FVector> PathPositions;
	FVector LastTraceDirection;

	UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), HitResult, PathPositions, LastTraceDirection, GetOwner()->GetActorLocation(),
																	OutVelocity, false, 5.f, ECollisionChannel::ECC_EngineTraceChannel1, true,
																	TArray<AActor*> { GetOwner() }, ShowTrajectory == true ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, 5.f, SimStep, MaxSimTime);

	return PathPositions;

}
