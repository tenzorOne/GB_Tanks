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

// Cannon fire functional
void ACannon::Fire()
{
	if (Ammo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, "NOT ENOUGH AMMO!");
		return;
	}
	
	if (!ReadyToFire)
		return;
	ReadyToFire = false;

	if (CannonType == ECannonType::FireProjectile)
	{
		Ammo--;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("Ammo: ") + FString::FromInt(Ammo) + FString("/") + FString::FromInt(MaxAmmo));
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Fire: Projectile");
	}
	else if (CannonType == ECannonType::FireTrace)
	{
		Ammo--;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Ammo: ") + FString::FromInt(Ammo) + FString("/") + FString::FromInt(MaxAmmo));
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, "Fire: Hit-Scan");
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);

}

// Automatic cannon fire functional
void ACannon::AutomaticFire()
{
	if (ReadyToFire)
	{
		if (Ammo != 0)
		{
			Ammo--;
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString("Ammo: ") + FString::FromInt(Ammo) + FString("/") + FString::FromInt(MaxAmmo));
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, "Fire: Automatic Projectile");
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::AutomaticFire, 1.f / FireRate, false);
		}
		else
			GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, "NOT ENOUGH AMMO!");
	}
	else
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);

}

// Special cannon fire functional (on RMB)
void ACannon::FireSpecial()
{
	if (Ammo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, "NOT ENOUGH AMMO!");
		return;
	}
	
	if (!ReadyToFire)
		return;
	ReadyToFire = false;

	Ammo--;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, FString("Ammo: ") + FString::FromInt(Ammo) + FString("/") + FString::FromInt(MaxAmmo));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, "SPECIAL FIRE");
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);

}

void ACannon::Reload()
{
	ReadyToFire = true;

}

void ACannon::StopAutomaticFire()
{
	ReadyToFire = false;

}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;

}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	MaxAmmo = Ammo;

}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

