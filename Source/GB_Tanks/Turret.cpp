// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "TankPlayerController.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"

// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collider"));
	HitCollider->SetupAttachment(TurretMesh);
	
	TargetsCheckVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Target's Check Volume"));
	TargetsCheckVolume->SetupAttachment(TurretMesh);
	TargetsCheckVolume->SetSphereRadius(TargetingRange * 2.f);
	TargetsCheckVolume->OnComponentBeginOverlap.AddDynamic(this, &ATurret::BeginComponentOverlap);

}


void ATurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);

}

void ATurret::BeginComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bPlayersAlly)
	{
		if (OtherActor->ActorHasTag("Enemy") && OtherActor != PlayerPawn)
		{
			CurrentTarget = OtherActor;
		}
		else
		{
			CurrentTarget = nullptr;
		}
	}
	else
	{
		CurrentTarget = PlayerPawn;
	}

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
	if (TargetInRange())
	{
		RotateToTarget();
	}
	
	if (CanFire() && TargetInRange())
	{
		if (CurrentTarget)
		{
			StartFire_WithCurrentTarget(CurrentTarget);
		}
		else
		{
			StartFire();
		}

	}
	else
	{
		StopFire();
	}
	
}

void ATurret::RotateToTarget()
{
	if (CurrentTarget)
	{
		RotateTurretTo(CurrentTarget->GetActorLocation());
	}

}

bool ATurret::TargetInRange()
{
	if (CurrentTarget)
	{
		return FVector::Distance(CurrentTarget->GetActorLocation(), GetActorLocation()) <= TargetingRange;
	}
	
	return false;

}

bool ATurret::DetectTargetVisibility()
{
	FVector TargetPosition;
	if (CurrentTarget)
	{
		TargetPosition = CurrentTarget->GetActorLocation();
	}
	else
	{
		TargetPosition = FVector(0.f);
	}
	FVector ViewPosition = GetViewPosition();

	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(Cannon);
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, ViewPosition, TargetPosition, ECollisionChannel::ECC_Visibility, TraceParams))
	{

		if (HitResult.Actor.Get())
		{
			//DrawDebugLine(GetWorld(), ViewPosition, HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
			return HitResult.Actor.Get() == CurrentTarget;
		}
	}
	//DrawDebugLine(GetWorld(), ViewPosition, PlayerPosition, FColor::Cyan, false, 0.5f, 0, 10);
	return false;

}

bool ATurret::CanFire()
{
	if (!DetectTargetVisibility())
	{
		return false;
	}
	
	FVector TargetDirection = TurretMesh->GetForwardVector();
	FVector DirectionToTarget = CurrentTarget->GetActorLocation() - TurretMesh->GetComponentLocation();
	DirectionToTarget.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetDirection, DirectionToTarget)));
	
	return AimAngle <= Accurency;

}
