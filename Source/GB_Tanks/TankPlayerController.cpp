// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = false;

}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATankPlayerController::StopFire);
	InputComponent->BindAction("SwitchCannon", IE_Pressed, this, &ATankPlayerController::SwitchCannon);

}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());

}

// Mouse position to world
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector PawnPosition = TankPawn->GetActorLocation();
	DeprojectMousePositionToWorld(MousePosition, MouseDirection);
	float DistBetweenMouseAndPawn = UKismetMathLibrary::GetPointDistanceToLine(MousePosition, PawnPosition, MouseDirection);
	MousePosition.Z = PawnPosition.Z;
	FVector MousePositionToWorld = MousePosition - PawnPosition;
	MousePositionToWorld.Normalize();
	MousePosition = PawnPosition + MousePositionToWorld * DistBetweenMouseAndPawn;
	//DrawDebugLine(GetWorld(), PawnPosition, MousePosition, FColor::Blue, false, 0.f, 0.f, 5.f);

}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->StartFire();
	}

}

void ATankPlayerController::StopFire()
{
	if (TankPawn)
	{
		TankPawn->StopFire();

		if (OnMouseButtonUp.IsBound())
		{
			OnMouseButtonUp.Broadcast();
		}
	}

}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}

}

void ATankPlayerController::RotateRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(AxisValue);
	}

}

void ATankPlayerController::SwitchCannon()
{
	if (TankPawn)
	{
		TankPawn->SwitchCannon();
	}

}
