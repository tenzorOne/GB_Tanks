// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon Setup Point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

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
	
	TempStopFactor = GetDefaultStopFactor();
	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon(MainCannon, MainCannonClass);
	SetupCannon(SecondCannon, SecondCannonClass);
}

// Spawn and setup cannon on tank
void ATankPawn::SetupCannon(ACannon* InCurrentCannon, TSubclassOf<ACannon> InCannonClass)
{
	if (InCurrentCannon)
	{
		InCurrentCannon->Destroy();
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = this;
	InCurrentCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, SpawnParameters);
	if (bIsMainCannon)
	{
		MainCannon = InCurrentCannon;
		InCurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CurrentCannon = InCurrentCannon;
		bIsMainCannon = false;
	}
	else
	{
		SecondCannon = InCurrentCannon;
		InCurrentCannon->SetActorHiddenInGame(true);
		InCurrentCannon->SetActorLocation({ 0.f, 0.f, 0.f });
		bIsMainCannon = true;
	}

}

void ATankPawn::InstallNewCannon(TSubclassOf<ACannon> InCannonClass)
{
	if (CurrentCannon)
	{
		CurrentCannon->Destroy();
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = this;
	SpawnParameters.Owner = this;
	CurrentCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, SpawnParameters);
	if (bIsMainCannon)
	{
		MainCannon = CurrentCannon;
	}
	else
	{
		SecondCannon = CurrentCannon;
	}

	CurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::ChangeWeapon()
{
	if (CurrentCannon)
	{
		if (bIsMainCannon)
		{
			CurrentCannon->SetActorHiddenInGame(true);
			CurrentCannon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentCannon->SetActorLocation({ 0.f, 0.f, 0.f });
			CurrentCannon = SecondCannon;
			CurrentCannon->SetActorHiddenInGame(false);
			CurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			bIsMainCannon = false;
		}
		else
		{
			CurrentCannon->SetActorHiddenInGame(true);
			CurrentCannon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentCannon->SetActorLocation({ 0.f, 0.f, 0.f });
			CurrentCannon = MainCannon;
			CurrentCannon->SetActorHiddenInGame(false);
			CurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			bIsMainCannon = true;
		}
	}
}

// Call the correct method based on the current Cannon type
void ATankPawn::Fire()
{
	if (CurrentCannon)
	{
		if (CurrentCannon->GetCannonType() == ECannonType::FireAutomaticProjectile)
		{
			CurrentCannon->AutomaticFire();
		}
		else
		{
			CurrentCannon->Fire();
		}
	}

}

// Stop automatic fire based on Fire Action-input (IE_Released)
void ATankPawn::StopAutomaticFire()
{
	if (CurrentCannon && CurrentCannon->GetCannonType() == ECannonType::FireAutomaticProjectile)
	{
		CurrentCannon->StopAutomaticFire();
	}

}

void ATankPawn::FireSpecial()
{
	if (CurrentCannon)
	{
		CurrentCannon->FireSpecial();
	}

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank Movement //
	FVector CurrentLocation = GetActorLocation();
	FVector PreviousLocation = PreviousLocation == CurrentLocation ? PreviousLocation : CurrentLocation;
	FVector ForwardVector = GetActorForwardVector();
	if (TargetForwardAxisValue == 0)
	{
		MovementInterp = MovementSmoothnes = FMath::FInterpTo(MovementSmoothnes, 0.f, DeltaTime, 6.f);

		AddActorWorldOffset(UKismetMathLibrary::Multiply_VectorFloat(UKismetMathLibrary::Multiply_VectorFloat(ForwardVector, LastForwardAxisValue),
			                FMath::FInterpConstantTo(StopInertiaFactor = FMath::Clamp(StopInertiaFactor - 0.01f, 0.f, StopInertiaFactor), 0.f, DeltaTime, MovementMomentum)), true);
	}
	else
	{
		StopInertiaFactor = FMath::FInterpTo(StopInertiaFactor, TempStopFactor, DeltaTime, StopFactorInterpSpeed);
		LastForwardAxisValue = TankController->GetInputAxisValue("MoveForward");
		FVector MovePosition = FMath::VInterpTo(PreviousLocation, CurrentLocation + (ForwardVector * TargetForwardAxisValue) * MoveSpeed, DeltaTime,
							   MovementInterp = FMath::FInterpConstantTo(0.f, UKismetMathLibrary::SafeDivide(MoveSpeed, 100.f), DeltaTime,
							   MovementSmoothnes = FMath::Clamp(MovementSmoothnes + MovementMomentum, 0.f, UKismetMathLibrary::SafeDivide(MoveSpeed, 100.f))));
		SetActorLocation(MovePosition, true);
	}
	//

	// Tank Rotation//
	if (bUseBaseConstantRotationSmoothness)
	{
		CurrentRightAxisValue = FMath::FInterpConstantTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
	}
	else
	{
		CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
	}
	float YawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
    YawRotation += GetActorRotation().Yaw;
	SetActorRotation({ 0.f, YawRotation, 0.f });
	//

	// TurretRotation //
	if (TankController)
	{
		FVector MousePos = TankController->GetMousePos();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
		FRotator CurrRotation = TurretMesh->GetComponentRotation();
		TargetRotation.Pitch = CurrRotation.Pitch;
		TargetRotation.Roll = CurrRotation.Roll;
		if (bUseTurretConstantRotationSmoothness)
		{
			TargetRotation = FMath::RInterpConstantTo(CurrRotation, TargetRotation, DeltaTime, TurretRotationSmoothness);
		}
		else
		{
			TargetRotation = FMath::RInterpTo(CurrRotation, TargetRotation, DeltaTime, TurretRotationSmoothness);
		}
		TurretMesh->SetWorldRotation(TargetRotation);
	}
	//

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
		AxisValue = TargetForwardAxisValue >= 0 ? AxisValue : AxisValue * -1;
	}
	TargetRightAxisValue = AxisValue;

}
