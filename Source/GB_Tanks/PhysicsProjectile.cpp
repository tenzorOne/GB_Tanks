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
		if (TragectoryPointIndex >= CurrentTrajectory.Num() - 2)
		{
			Explode();
			if (OnDeathParticleEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticleEffect, GetActorLocation(), FRotator(0.f), FVector(3.f), true, EPSCPoolMethod::AutoRelease, true);
			}
			Destroy();
		}
		else
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
			SetActorRotation(NewRotation);
		}
	}

}

void APhysicsProjectile::Explode()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;

	bool SweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		StartPos,
		EndPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		Params
	);

	//GetWorld()->DebugDrawTraceTag = "Explode Trace";

	if (SweepResult)
	{
		for (FHitResult HitResult : AttackHit)
		{
			AActor* OtherActor = HitResult.GetActor();
			if (!OtherActor)
			{
				continue;
			}

			IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
			if (DamageTakerActor)
			{
				DamageData.DamageValue = Damage;
				DamageData.HitLocation = HitResult.Location;
				DamageData.Instigator = GetOwner();
				DamageData.DamageMaker = this;

				DamageTakerActor->TakeDamage(DamageData);
			}
			else
			{
				UPrimitiveComponent* PhysicsMesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
				if (PhysicsMesh)
				{
					if (PhysicsMesh->IsSimulatingPhysics())
					{
						FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
						ForceVector.Normalize();
						PhysicsMesh->AddImpulse(ForceVector * PushForce, NAME_None, true);
					}
				}
			}

		}
	}

}
