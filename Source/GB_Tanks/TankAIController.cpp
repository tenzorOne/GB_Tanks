// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	Initialize();

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TankPawn)
	{
		Initialize();
	}

	if (!TankPawn)
	{
		return;
	}

	if (CurrentPatrolPointIndex == INDEX_NONE)
	{
		TankPawn->MoveForward(0.f);
		return;
	}
	
	TankPawn->MoveForward(1.f);

	float RotationValue = CalculateRotatingValue();
	TankPawn->RotateRight(RotationValue);

	Targeting();

}

void ATankAIController::Initialize()
{
	TankPawn = Cast<ATankPawn>(GetPawn());
	
	if (TankPawn)
	{
		PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		MovementAccuracy = TankPawn->GetMovementAccuracy();
		FVector PawnLocation = TankPawn->GetActorLocation();
		TArray<FVector> Points = TankPawn->GetPatrollingPoints();
		
		for (FVector Point : Points)
		{
			PatrollingPoints.Add(Point);
		}

		CurrentPatrolPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;
	}

}

float ATankAIController::CalculateRotatingValue()
{
	FVector CurrentPoint = PatrollingPoints[CurrentPatrolPointIndex];
	FVector PawnLocation = TankPawn->GetActorLocation();
	if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccuracy)
	{
		CurrentPatrolPointIndex++;
		if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
		{
			CurrentPatrolPointIndex = 0;
		}
	}

	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	FVector ForwardDirection = TankPawn->GetActorForwardVector();
	FVector RightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
	float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection)));

	float RotationValue = 0.f;
	if (ForwardAngle > 5.f)
	{
		RotationValue = 1.f;
	}
	if (RightAngle > 90.f)
	{
		RotationValue = -RotationValue;
	}

	return RotationValue;

}

void ATankAIController::Targeting()
{
	if (CanFire())
	{
		TankPawn->StartFire();
	}
	else
	{
		TankPawn->StopFire();
		RotateToPlayer();
	}

}

void ATankAIController::RotateToPlayer()
{
	if (IsPlayerInRange())
	{
		TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
	}

}

bool ATankAIController::IsPlayerInRange()
{
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;

}

bool ATankAIController::DetectPlayerVisibility()
{
	FVector PlayerPos = PlayerPawn->GetActorLocation();
	FVector EyesPos = TankPawn->GetViewPosition();

	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		if (HitResult.Actor.Get())
		{
			return HitResult.Actor.Get() == PlayerPawn;
		}
	}
	return false;

}

bool ATankAIController::CanFire()
{
	if (IsPlayerInRange() && DetectPlayerVisibility())
	{
		FVector TargetingDirection = TankPawn->GetTurretForwardVector();
		FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
		DirectionToPlayer.Normalize();
		float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));

		return AimAngle <= Accuracy;
	}
	else
	{
		return false;
	}

}
