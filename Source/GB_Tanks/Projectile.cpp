// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "DamageTaker.h"
#include "Particles/ParticleSystem.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>

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
					OnDestroyTarget.Broadcast();
				}
			}
		}
	}
	else
	{
		if (OnDeathParticleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticleEffect, OverlappedComp->GetComponentLocation(), FRotator(0.f), FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);
		}
	}
	Destroy();

}

void AProjectile::Move()
{
	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(NextPosition);

}
