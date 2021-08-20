// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector MovePosition = CurrentLocation + (ForwardVector * TargetForwardAxisValue) * MoveSpeed * DeltaTime;
	SetActorLocation(MovePosition, true);

	if(bUseConstantRotationSmoothness)
		CurrentRightAxisValue = FMath::FInterpConstantTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
	else
		CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
	float YawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
    YawRotation += GetActorRotation().Yaw;
	SetActorRotation({ 0.f, YawRotation, 0.f });

	UE_LOG(LogTemp, Warning, TEXT("TargetForwardAxisValue = %f"), TargetForwardAxisValue);

}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;

}

void ATankPawn::RotateRight(float AxisValue)
{
	if (TargetForwardAxisValue != 0)
	{
		AxisValue = TargetForwardAxisValue == 1 ? AxisValue : AxisValue * -1;
	}
	TargetRightAxisValue = AxisValue;

}