// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	MovementAccuracy = TankPawn->GetMovementAccuracy();
	TArray<FVector> Points = TankPawn->GetPatrollingPoints();
	FVector PawnLocation = TankPawn->GetActorLocation();
	
	for (FVector Point : Points)
	{
		PatrollingPoints.Add(Point + PawnLocation);
	}

	CurrentPatrolPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = TankPawn->GetViewPosition();

	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(this);
	traceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{

		if (hitResult.Actor.Get())
		{
			//DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
			return hitResult.Actor.Get() == PlayerPawn;
		}
	}
	//DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);
	return false;

}

bool ATankAIController::CanFire()
{
	if (IsPlayerInRange())
	{
		if (!DetectPlayerVisibility())
		{
			return false;
		}
	}

	FVector TargetingDirection = TankPawn->GetTurretForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
	
	return AimAngle <= Accuracy;

}
