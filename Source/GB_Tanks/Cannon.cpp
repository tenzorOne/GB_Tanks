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
void ACannon::StartFire()
{
	if (CurrentAmmo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 2, FColor::Orange, "NOT ENOUGH AMMO!");
		return;
	}
	
	if (bReadyToFire)
	{
		if (CannonType == ECannonType::FireProjectile)
		{
			bReadyToFire = false;
			
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			
			if (Projectile)
			{
				Projectile->SetInstigator(GetInstigator());
				Projectile->Start();
				--CurrentAmmo;
			}

			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Fire: Projectile");

			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
		}
		else if (CannonType == ECannonType::FireAutomaticProjectile)
		{
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			
			if (Projectile)
			{
				Projectile->SetInstigator(GetInstigator());
				Projectile->Start();
				--CurrentAmmo;
			}

			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, "Fire: Automatic Projectile");

			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::StartFire, 1.f / FireRate, false);

		}
		else if (CannonType == ECannonType::FireTrace)
		{
			bReadyToFire = false;

			FHitResult HitResult;
			FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
			TraceParams.bTraceComplex = true;
			TraceParams.bReturnPhysicalMaterial = false;
			FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
			FVector TraceEnd = ProjectileSpawnPoint->GetForwardVector() * FireRange + TraceStart;

			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
			{
				DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
				if (HitResult.Actor.Get())
				{
					HitResult.Actor.Get()->Destroy();
				}
				--CurrentAmmo;
			}
			else
			{
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5);
				--CurrentAmmo;
			}

			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, "Fire: Hit-Scan");

			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
		}
	}

}

void ACannon::StopFire()
{
	if (CannonType == ECannonType::FireAutomaticProjectile)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	}
}

// Special cannon fire functional (on RMB)
void ACannon::FireSpecial()
{
	if (CurrentAmmo == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 2, FColor::Orange, "NOT ENOUGH AMMO!");
		return;
	}
	
	if (bReadyToFire)
	{
		bReadyToFire = false;

		--CurrentAmmo;
		
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, FString("Ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, "SPECIAL FIRE");
		
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
	}

}

void ACannon::AddAmmo(int32 AmmoToAdd)
{
	AmmoToAdd = FMath::Clamp(AmmoToAdd, 0, MaxAmmo - CurrentAmmo);
	CurrentAmmo += AmmoToAdd;
	GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Yellow, FString("You picked up ") + FString::FromInt(AmmoToAdd) + FString(" and you current ammo: ") + FString::FromInt(CurrentAmmo) + FString("/") + FString::FromInt(MaxAmmo));

}

void ACannon::Reload()
{
	bReadyToFire = true;

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
