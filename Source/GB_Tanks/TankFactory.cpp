// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "MapLoader.h"
#include "Particles/ParticleSystem.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(RootComponent);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon Setup Point"));
	TankSpawnPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collider"));
	HitCollider->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);

}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(false);
	}

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true);
	
}

void ATankFactory::SpawnNewTank()
{
	FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1.f));
	ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (NewTank)
	{
		if (OnTankSpawnParticleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnTankSpawnParticleEffect, TankSpawnPoint->GetComponentLocation(), TankSpawnPoint->GetComponentRotation(), FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);
		}
		NewTank->SetPatrollingPoints(TankWayPoints);
		UGameplayStatics::FinishSpawningActor(NewTank, SpawnTransform);
	}

}

void ATankFactory::TakeDamage(FDamageData& DamageData)
{
	if (OnHitParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticleEffect, DamageData.HitLocation, FRotator(0.f), FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);
	}
	HealthComponent->TakeDamage(DamageData);

}

void ATankFactory::Die()
{
	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(true);
	}

	if (OnDeathParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticleEffect, GetActorLocation(), FRotator(0.f), FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);
	}
	
	Destroy();

}
