// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;

}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATankPlayerController::StopFire);
	InputComponent->BindAction("AlternativeFire", IE_Pressed, this, &ATankPlayerController::FireSpecial);
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

	FVector MouseDirection;
	DeprojectMousePositionToWorld(MousePos, MouseDirection);
	FVector PawnPos = TankPawn->GetActorLocation();
	MousePos.Z = PawnPos.Z;
	FVector MouseDir = MousePos - PawnPos;
	MouseDir.Normalize();
	MousePos = PawnPos + MouseDir * 1000.f;
	DrawDebugLine(GetWorld(), PawnPos, MousePos, FColor::Blue, false, 0.f, 0.f, 5.f);

}

void ATankPlayerController::Fire()
{
	TankPawn->StartFire();

}

void ATankPlayerController::StopFire()
{
	TankPawn->StopFire();

}

void ATankPlayerController::FireSpecial()
{
	TankPawn->FireSpecial();

}

void ATankPlayerController::MoveForward(float AxisValue)
{
	TankPawn->MoveForward(AxisValue);

}

void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);

}

void ATankPlayerController::SwitchCannon()
{
	TankPawn->SwitchCannon();
}
