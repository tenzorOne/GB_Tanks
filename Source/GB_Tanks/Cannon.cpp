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
#include "Components/AudioComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "AmmoBox.h"


ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	CannonMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Cannon Mesh"));
	CannonMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);
	ShootEffect->bAutoActivate = false;

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Shoot Audio"));
	AudioEffect->SetupAttachment(RootComponent);
	AudioEffect->bAutoActivate = false;

}

void ACannon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (CannonType != ECannonType::FireProjectile)
	{
		bAutomaticCannon = false;
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;

	if (CannonType == ECannonType::FireProjectile && bAutomaticCannon)
	{
		bAutomaticFire = true;
	}

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
			bReadyToFire = false;
			
			ShootEffect->ActivateSystem();
			AudioEffect->Play();

			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			if (Projectile)
			{
				Projectile->OnDestroyTarget.AddUObject(this, &ACannon::TargetDestroyed);
				Projectile->SetInstigator(GetInstigator());
				Projectile->SetOwner(this);
				Projectile->Start();
				--CurrentAmmo;
			}

			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
		}
		else
		{
			bReadyToFire = false;
			AudioEffect->Play();
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
						DamageData.HitLocation = HitResult.Location;
						DamageData.DamageMaker = this;
						DamageData.Instigator = GetInstigator();
						DamageTaker->TakeDamage(DamageData);
						
						if (DamageData.bTargetKilled)
						{
							GetWorld()->SpawnActor<AAmmoBox>(AmmoBoxForSpawn, DamageData.HitLocation, FRotator(0.f));
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

		if (ShootShake && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootShake);
		}
	}

}

void ACannon::StopFire()
{
	if (CannonType == ECannonType::FireProjectile && bAutomaticCannon)
	{
		bReadyToFire = true;
		bAutomaticFire = true;
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
	if (CannonType == ECannonType::FireProjectile && bAutomaticCannon && bAutomaticFire)
	{
		StartFire();
	}
	else
	{
		StopFire();
	}

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
