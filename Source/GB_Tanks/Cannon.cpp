// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
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
	if (CurrentAmmo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, "NOT ENOUGH AMMO!");
		return;
	}
	
	if (!bReadyToFire)
	{
		return;
	}
	bReadyToFire = false;

	if (CannonType == ECannonType::FireProjectile)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}

		CurrentAmmo--;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Fire: Projectile");
	}
	else if (CannonType == ECannonType::FireTrace)
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParamas = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		TraceParamas.bTraceComplex = true;
		TraceParamas.bReturnPhysicalMaterial = false;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = ProjectileSpawnPoint->GetForwardVector() * FireRange + TraceStart;
		
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParamas))
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (HitResult.Actor.Get())
			{
				HitResult.Actor.Get()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5);
		}
		
		CurrentAmmo--;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, "Fire: Hit-Scan");
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);

}

// Automatic cannon fire functional
void ACannon::AutomaticFire()
{
	if (bReadyToFire)
	{
		if (CurrentAmmo != 0)
		{
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			if (Projectile)
			{
				Projectile->Start();
			}
			
			CurrentAmmo--;
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
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
	if (CurrentAmmo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, "NOT ENOUGH AMMO!");
		return;
	}
	
	if (!bReadyToFire)
	{
		return;
	}
	bReadyToFire = false;

	CurrentAmmo--;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, "SPECIAL FIRE");
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);

}

void ACannon::Reload()
{
	bReadyToFire = true;

}

void ACannon::StopAutomaticFire()
{
	bReadyToFire = false;

}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire;

}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;

}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

