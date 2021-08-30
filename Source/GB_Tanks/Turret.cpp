// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collider"));
	HitCollider->SetupAttachment(TurretMesh);

}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);

}

void ATurret::Destroyed()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

}

void ATurret::Targeting()
{
	if (IsPlayerInRange())
	{
		RotateToPlayer();
	}

	if (Cannon && Cannon->IsReadyToFire() && CanFire())
	{
		Fire();
	}

}

void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), PlayerPawn->GetActorLocation());
	TargetRotation.Roll = TurretMesh->GetComponentRotation().Roll;
	TurretMesh->SetWorldRotation(TargetRotation);

}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;

}

bool ATurret::CanFire()
{
	FVector TargetDirection = TurretMesh->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - TurretMesh->GetComponentLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetDirection, DirectionToPlayer)));
	
	return AimAngle <= Accurency;

}

void ATurret::Fire()
{
	if (Cannon)
	{
		Cannon->StartFire();
	}

}
