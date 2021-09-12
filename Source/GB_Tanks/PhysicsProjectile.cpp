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
	MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep, 0);
	if (bShowTrajectory)
	{
		for (FVector Position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), Position, 5, 8, FColor::Purple, true, 1, 0, 2);
		}
	}

	TragectoryPointIndex = 0;
	Super::Start();

}

void APhysicsProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TragectoryPointIndex] - GetActorLocation();
	CurrentMoveVector.Normalize();
	FVector NewLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(NewLocation);
	if (FVector::Distance(NewLocation, CurrentTrajectory[TragectoryPointIndex]) <= MoveAccurency)
	{
		TragectoryPointIndex++;
		if (TragectoryPointIndex >= CurrentTrajectory.Num())
		{
			Destroy();
		}
		else
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
			SetActorRotation(NewRotation);
		}
	}

}
