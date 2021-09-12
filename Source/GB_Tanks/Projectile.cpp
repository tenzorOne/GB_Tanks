// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "DamageTaker.h"
#include "Particles/ParticleSystem.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>
#include "AmmoBox.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	SetLifeSpan(FlyRange / MoveSpeed);

}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bExplosiveProjectile)
	{
		Explode();
	}
	else
	{
		if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(OtherActor))
		{
			if (OtherActor != GetInstigator())
			{
				DamageData.DamageValue = Damage;
				DamageData.HitLocation = OverlappedComp->GetComponentLocation();
				DamageData.DamageMaker = this;
				DamageData.Instigator = GetInstigator();
				DamageTaker->TakeDamage(DamageData);

				if (DamageData.bTargetKilled)
				{
					if (OnDestroyTarget.IsBound())
					{
						if (ACannon* MyOwner = Cast<ACannon>(GetOwner()))
						{
							GetWorld()->SpawnActor<AAmmoBox>(MyOwner->GetAmmoBoxForSpawn(), DamageData.HitLocation, FRotator(0.f));
						}
						OnDestroyTarget.Broadcast();
					}
				}
			}
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
					PhysicsMesh->AddImpulseAtLocation(ForceVector * PushForce, SweepResult.ImpactPoint);
				}
			}

			if (OnDeathParticleEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticleEffect, OverlappedComp->GetComponentLocation(), FRotator(0.f), FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);
			}
		}
		Destroy();
	}

}

void AProjectile::Move()
{
	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(NextPosition);

}

void AProjectile::Explode()
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

			IDamageTaker* DamageTaker = Cast<IDamageTaker>(OtherActor);
			if (DamageTaker)
			{
				DamageData.DamageValue = Damage;
				DamageData.HitLocation = RootComponent->GetComponentLocation();
				DamageData.DamageMaker = this;
				DamageData.Instigator = GetInstigator();
				DamageTaker->TakeDamage(DamageData);
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
						PhysicsMesh->AddImpulseAtLocation(ForceVector * PushForce, DamageData.HitLocation);
					}
				}
			}
		}
		
		if (OnDeathParticleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticleEffect, RootComponent->GetComponentLocation(), FRotator(0.f), FVector(ExplodeRadius / 100.f), true, EPSCPoolMethod::AutoRelease, true);
		}
	}

	Destroy();

}