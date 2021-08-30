// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerMachineMaster.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TankPlayerController.h"
#include "Projectile.h"
#include "HealthComponent.h"

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

	SetupCannon(CannonClass);

}

// Called every frame
void ATowerMachineMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void ATowerMachineMaster::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);

}

void ATowerMachineMaster::Die(AActor* DamageMaker)
{
	if (Cast<AProjectile>(DamageMaker) || Cast<ACannon>(DamageMaker))
	{		
		EarningPoints(5);

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Destroy Something and earning points! Current points: ") + FString::FromInt(CurrentPoints));
	}
	Destroy();

}

void ATowerMachineMaster::EarningPoints(int32 Points)
{
	CurrentPoints = Points;

}