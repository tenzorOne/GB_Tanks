// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerMachineMaster.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TankPlayerController.h"
#include "Projectile.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystem.h"
#include <Kismet/GameplayStatics.h>
#include "DrawDebugHelpers.h"

// Sets default values
ATowerMachineMaster::ATowerMachineMaster()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon Setup Point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDie.AddUObject(this, &ATowerMachineMaster::Die);

}

// Called when the game starts or when spawned
void ATowerMachineMaster::BeginPlay()
{
	Super::BeginPlay();

	TakeDamageDelegate.BindUFunction(this, FName("EventTakeDamage"));
	SetupCannon(CannonClass);

}

// Called every frame
void ATowerMachineMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ATowerMachineMaster::GetViewPosition()
{
	return CannonSetupPoint->GetComponentLocation();

}

// Called to bind functionality to input
void ATowerMachineMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATowerMachineMaster::SetupCannon(TSubclassOf<ACannon> CannonClassToSetup)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClassToSetup, SpawnParameters);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void ATowerMachineMaster::RotateTurretTo(FVector TargetPosition)
{	
	FRotator CurrentRotation;
	FRotator TargetRotation;

	if (bRotationByCannonPosition)
	{
		CurrentRotation = CannonSetupPoint->GetComponentRotation();
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(CannonSetupPoint->GetComponentLocation(), TargetPosition);
	}
	else
	{
		CurrentRotation = TurretMesh->GetComponentRotation();
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetPosition);
	}
	
	TargetRotation.Roll = CurrentRotation.Roll;

	if (bUseTurretConstantInterpRotation)
	{
		TargetRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TurretRotationSpeed);
		//TargetRotation = UKismetMathLibrary::RLerp(CurrRotation, TargetRotation, TurretRotationSmoothness, true);
	}
	else
	{
		TargetRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TurretRotationSpeed);
		//TargetRotation = UKismetMathLibrary::RLerp(CurrRotation, TargetRotation, TurretRotationSmoothness, true);
	}
	TurretMesh->SetWorldRotation(TargetRotation);

	if (this == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		DrawDebugLine(GetWorld(), CannonSetupPoint->GetComponentLocation(), TargetPosition, FColor::Blue, false, -1.f, 0, 10.f);
	}

}

void ATowerMachineMaster::StartFire()
{
	Cannon->bAutomaticFire = true;
	Cannon->StartFire();

}

void ATowerMachineMaster::StopFire()
{
	Cannon->bAutomaticFire = false;

}

void ATowerMachineMaster::TakeDamage(FDamageData& DamageData)
{
	if (OnHitParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticleEffect, DamageData.HitLocation, FRotator(0.f), FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);
	}
	TakeDamageDelegate.ExecuteIfBound();
	HealthComponent->TakeDamage(DamageData);

}

void ATowerMachineMaster::EarningPoints()
{
	CurrentPoints += 2;

}

void ATowerMachineMaster::Die()
{
	if (OnDeathParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticleEffect, GetActorLocation(), FRotator(0.f), FVector(1.f), true, EPSCPoolMethod::AutoRelease, true);
	}
	Destroy();

}
