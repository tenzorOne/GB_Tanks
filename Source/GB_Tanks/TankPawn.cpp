// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "TankPlayerController.h"
#include "TankAIController.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ATankPawn::ATankPawn()
{	
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


void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TankController = Cast<ATankPlayerController>(GetController());
	TankAIController = Cast<ATankAIController>(GetController());

	FirstCannon = Cannon;
	SetupAnotherCannon(SecondCannonClass, false);
	
	TempStopFactor = GetDefaultStopFactor();

}

void ATankPawn::SetupAnotherCannon(TSubclassOf<ACannon> CannonClassToSetup, bool bPickupNewCannon)
{
	if (Cannon)
	{		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		ACannon* TempCannon = GetWorld()->SpawnActor<ACannon>(CannonClassToSetup, SpawnParameters);
		TempCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		TempCannon->SetActorHiddenInGame(!bPickupNewCannon ? true : false);
		TempCannon->SetActorEnableCollision(!bPickupNewCannon ? false : true);

		if (bPickupNewCannon)
		{
			Cannon->Destroy();
			if (Cannon == FirstCannon)
			{
				Cannon = FirstCannon = TempCannon;
			}
			else
			{
				Cannon = SecondCannon = TempCannon;
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
	if (Cannon)
	{
		if (!bSwitchCannon)
		{
			Cannon->SetActorHiddenInGame(true);
			Cannon->SetActorEnableCollision(false);
			Cannon->SetActorLocation({ 0.f, 0.f, 0.f });
			Cannon = SecondCannon;
			Cannon->SetActorHiddenInGame(false);
			Cannon->SetActorEnableCollision(true);
			Cannon->SetActorLocation(CannonSetupPoint->GetComponentLocation());
			bSwitchCannon = true;
		}
		else
		{
			Cannon->SetActorHiddenInGame(true);
			Cannon->SetActorEnableCollision(false);
			Cannon->SetActorLocation({ 0.f, 0.f, 0.f });
			Cannon = FirstCannon;
			Cannon->SetActorHiddenInGame(false);
			Cannon->SetActorEnableCollision(true);
			Cannon->SetActorLocation(CannonSetupPoint->GetComponentLocation());
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
	if (TankController)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), TankController, EQuitPreference::Quit, false);
	}

}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
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
	FVector MovePosition;
	
	if (TankController)
	{
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

			MovePosition = FMath::VInterpTo(PreviousLocation, CurrentLocation + (ForwardVector * TargetForwardAxisValue) * MoveSpeed, DeltaTime,
											MovementInterp = FMath::FInterpConstantTo(0.f, UKismetMathLibrary::SafeDivide(MoveSpeed, 100.f), DeltaTime,
											MovementSmoothnes = FMath::Clamp(MovementSmoothnes + MovementMomentum, 0.f, UKismetMathLibrary::SafeDivide(MoveSpeed, 100.f))));

			SetActorLocation(MovePosition, true);
		}
	}
	else if (TankAIController)
	{
		MovePosition = CurrentLocation + (ForwardVector * TargetForwardAxisValue) * MoveSpeed / 100;
		SetActorLocation(MovePosition, false);
	}
	//

	// Tank Rotation//
	if (TankController)
	{
		if (bUseBaseConstantRotationSmoothness)
		{
			CurrentRightAxisValue = FMath::FInterpConstantTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
		}
		else
		{
			CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmoothness);
		}

		float YawRotation = CurrentRightAxisValue * RotationSpeed * DeltaTime;
		YawRotation += GetActorRotation().Yaw;
		SetActorRotation({ 0.f, YawRotation, 0.f });
	}
	else if (TankAIController)
	{
		float YawRotation = TargetRightAxisValue * RotationSpeed * DeltaTime;
		YawRotation += GetActorRotation().Yaw;
		SetActorRotation({ 0.f, YawRotation, 0.f });
	}
	//

	// TurretRotation //
	if (TankController)
	{
		FVector MousePos = TankController->GetMousePos();
		RotateTurretTo(MousePos);
	}
	//

}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();

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
