// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
	
	TankController = Cast<ATankPlayerController>(GetController());

	FirstCannon = Cannon;
	ActiveCannon = FirstCannon;
	SetupAnotherCannon(SecondCannonClass, false);
	
	TempStopFactor = GetDefaultStopFactor();

}

void ATankPawn::SetupAnotherCannon(TSubclassOf<ACannon> CannonClassToSetup, bool bPickupNewCannon)
{
	if (ActiveCannon)
	{		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		ACannon* TempCannon = GetWorld()->SpawnActor<ACannon>(CannonClassToSetup, SpawnParameters);
		TempCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		TempCannon->SetActorHiddenInGame(!bPickupNewCannon ? true : false);
		TempCannon->SetActorEnableCollision(!bPickupNewCannon ? false : true);

		if (bPickupNewCannon)
		{
			ActiveCannon->Destroy();
			if (ActiveCannon == FirstCannon)
			{
				ActiveCannon = FirstCannon = TempCannon;
			}
			else
			{
				ActiveCannon = SecondCannon = TempCannon;
			}
		}
		else
		{
			SecondCannon = TempCannon;
		}
	}

}

void ATankPawn::SwitchCannon()
{
	if (ActiveCannon)
	{
		if (!bSwitchCannon)
		{
			ActiveCannon->SetActorHiddenInGame(true);
			ActiveCannon->SetActorEnableCollision(false);
			ActiveCannon->SetActorLocation({ 0.f, 0.f, 0.f });
			ActiveCannon = SecondCannon;
			ActiveCannon->SetActorHiddenInGame(false);
			ActiveCannon->SetActorEnableCollision(true);
			ActiveCannon->SetActorLocation(CannonSetupPoint->GetComponentLocation());
			bSwitchCannon = true;
		}
		else
		{
			ActiveCannon->SetActorHiddenInGame(true);
			ActiveCannon->SetActorEnableCollision(false);
			ActiveCannon->SetActorLocation({ 0.f, 0.f, 0.f });
			ActiveCannon = FirstCannon;
			ActiveCannon->SetActorHiddenInGame(false);
			ActiveCannon->SetActorEnableCollision(true);
			ActiveCannon->SetActorLocation(CannonSetupPoint->GetComponentLocation());
			bSwitchCannon = false;
		}
	}
	else
	{
		return;
	}

}

void ATankPawn::Die(AActor* DamageMaker)
{
	Destroy();
	UKismetSystemLibrary::QuitGame(GetWorld(), TankController, EQuitPreference::Quit, false);

}

// Call the correct method based on the current Cannon type
void ATankPawn::StartFire()
{
	if (ActiveCannon)
	{
		ActiveCannon->StartFire();
	}
}

// Stop automatic fire based on Fire Action-input (IE_Released)
void ATankPawn::StopFire()
{
	if (ActiveCannon)
	{
		ActiveCannon->StopFire();
	}
}

void ATankPawn::FireSpecial()
{
	if (ActiveCannon)
	{
		ActiveCannon->FireSpecial();
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
