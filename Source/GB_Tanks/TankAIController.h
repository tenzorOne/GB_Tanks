// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class GB_TANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		ATankPawn* TankPawn;
	UPROPERTY()
		APawn* PlayerPawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement|Patrol Points")
		TArray<FVector> PatrollingPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement|Accuracy")
		float MovementAccuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float Accuracy = 10.f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void Initialize();
	float CalculateRotatingValue();
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool DetectPlayerVisibility();
	bool CanFire();

public:
	int32 CurrentPatrolPointIndex = INDEX_NONE;

};
