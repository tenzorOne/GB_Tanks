// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "DrawDebugHelpers.h"
#include "PhysicalTrajectoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DamageTaker.h"
#include <Kismet/GameplayStatics.h>

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicalTrajectoryComponent>(TEXT("Physics Component"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Effect"));
	TrailEffect->SetupAttachment(RootComponent);

}

void APhysicsProjectile::Start()
{
	if (PhysicsComponent->SuggestVeloctiy(PhysicsComponent->GetCurrentTarget()->GetActorLocation()))
	{
		CurrentTrajectory = PhysicsComponent->GenerateTrajectory(SimStep, MaxSimTime, bShowTrajectory);
		TrajectoryPointIndex = 0;

		Super::Start();
	}

}

void APhysicsProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation();
	CurrentMoveVector.Normalize();
	FVector NewLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(NewLocation);
	if (FVector::Distance(NewLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MoveAccurency)
	{
		TrajectoryPointIndex++;
		if (TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			Destroy();
		}
		else
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TrajectoryPointIndex]);
			SetActorRotation(NewRotation);
		}
	}

}
