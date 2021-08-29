// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDamaged.AddUObject(this, &ATurret::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATurret::Die);

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
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));

}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;

}

bool ATurret::CanFire()
{
	FVector TargetDirection = TurretMesh->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
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

void ATurret::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);

}

void ATurret::Die()
{
	Destroy();

}

void ATurret::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}