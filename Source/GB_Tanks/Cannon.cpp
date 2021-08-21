// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	CannonMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Cannon Mesh"));
	CannonMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

}

void ACannon::Fire()
{
	if (!ReadyToFire)
		return;
	ReadyToFire = false;

	if (CannonType == ECannonType::FireProjectile)
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Fire: Projectile");
	else
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire: Hit-Scan");

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

