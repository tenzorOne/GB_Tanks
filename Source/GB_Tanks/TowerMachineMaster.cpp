// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerMachineMaster.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TankPlayerController.h"

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
	SpawnParameters.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClassToSetup, SpawnParameters);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}
