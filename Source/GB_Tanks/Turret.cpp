// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "TankPlayerController.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

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
	
	if (CanFire() && IsPlayerInRange())
	{
		StartFire();
	}
	
}

void ATurret::RotateToPlayer()
{
	RotateTurretTo(PlayerPawn->GetActorLocation());

}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;

}

bool ATurret::DetectPlayerVisibility()
{
	FVector PlayerPosition = PlayerPawn->GetActorLocation();
	FVector ViewPosition = GetViewPosition();

	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(Cannon);
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, ViewPosition, PlayerPosition, ECollisionChannel::ECC_Visibility, TraceParams))
	{

		if (HitResult.Actor.Get())
		{
			//DrawDebugLine(GetWorld(), ViewPosition, HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
			return HitResult.Actor.Get() == PlayerPawn;
		}
	}
	//DrawDebugLine(GetWorld(), ViewPosition, PlayerPosition, FColor::Cyan, false, 0.5f, 0, 10);
	return false;

}

bool ATurret::CanFire()
{
	if (!DetectPlayerVisibility())
	{
		return false;
	}
	
	FVector TargetDirection = TurretMesh->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - TurretMesh->GetComponentLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetDirection, DirectionToPlayer)));
	
	return AimAngle <= Accurency;

}
