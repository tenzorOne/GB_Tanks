// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "DamageTaker.h"
#include "IScorable.h"
#include "Engine/Engine.h"


ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	CannonMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Cannon Mesh"));
	CannonMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

}

void ACannon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (CannonType != ECannonType::FireProjectile)
	{
		bAutomaticCannon = false;
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;

}

// Cannon fire functional
void ACannon::StartFire()
{
	if (CurrentAmmo == 0)
	{
		return;
	}
	
	if (bReadyToFire)
	{
		if (CannonType == ECannonType::FireProjectile)
		{
			if (bAutomaticCannon)
			{
				bReadyToFire = true;
			}
			else
			{
				bReadyToFire = false;
			}
						
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			Projectile->OnDestroyTarget.AddUObject(this, &ACannon::TargetDestroyed);
			
			if (Projectile)
			{
				Projectile->SetInstigator(GetInstigator());
				Projectile->Start();
				--CurrentAmmo;
			}

			if (bAutomaticCannon)
			{
				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::StartFire, 1.f / FireRate, false);;
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
			}
		}
		else
		{
			bReadyToFire = false;
			--CurrentAmmo;

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
					if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(HitResult.Actor.Get()))
					{
						DamageData.DamageValue = FireDamage;
						DamageData.DamageMaker = this;
						DamageData.Instigator = GetInstigator();
						DamageTaker->TakeDamage(DamageData);
						
						if (DamageData.bTargetKilled)
						{
							TargetDestroyed();
						}
					}
					else
					{
						//HitResult.Actor.Get()->Destroy();
					}
				}
			}
			else
			{
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5);
			}

			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
		}
	}

}

void ACannon::StopFire()
{
	if (bAutomaticCannon)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	}

}

// Special cannon fire functional (on RMB)
void ACannon::FireSpecial()
{
	if (CurrentAmmo == 0)
	{
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

}

void ACannon::Reload()
{
	bReadyToFire = true;

}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire;

}

void ACannon::TargetDestroyed()
{	
	if (IIScorable* ActorToEarningPoints = Cast<IIScorable>(GetOwner()))
	{
		ActorToEarningPoints->EarningPoints();
	}
	
}
