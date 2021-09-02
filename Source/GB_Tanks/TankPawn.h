// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerMachineMaster.h"
#include "Cannon.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshCompinent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ATankAIController;
class ACannon;

UCLASS()
class GB_TANKS_API ATankPawn : public ATowerMachineMaster
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION()
		void MoveForward(float AxisValue);
	UFUNCTION()
		void RotateRight(float AxisValue);
	UFUNCTION()
		void SetupAnotherCannon(TSubclassOf<ACannon> CannonClassToSetup, bool bPickupNewCannon);
	UFUNCTION()
		FVector GetTurretForwardVector();
	UFUNCTION()
		void RotateTurretTo(FVector TargetPosition);
	UFUNCTION()
		void FireSpecial();
	UFUNCTION()
		void SwitchCannon();
	UFUNCTION()
		TArray<FVector> GetPatrollingPoints() { return PatrollingPoints; };
	UFUNCTION()
		float GetMovementAccuracy() { return MovementAccuracy; };
	UFUNCTION()
		float GetDefaultStopFactor() { TempStopFactor = StopInertiaFactor; return TempStopFactor; };
	UFUNCTION()
		virtual void Die(AActor* DamageMaker) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon", meta = (DisplayPriority = "2"))
		TSubclassOf<ACannon> SecondCannonClass = CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed", meta = (ClampMin = "0", ClampMax = "1"))
		float MovementMomentum = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed", meta = (ClampMin = "0", ClampMax = "2"))
		float StopInertiaFactor = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed", meta = (ClampMin = "0", ClampMax = "10"))
		float StopFactorInterpSpeed = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSmoothness = 8.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Rotation")
		float TurretRotationSmoothness = 8.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "Use Constant Rotation Smoothness", Category = "Movement|Speed")
		bool bUseBaseConstantRotationSmoothness = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "Use Constant Rotation Smoothness", Category = "Turret|Rotation")
		bool bUseTurretConstantRotationSmoothness = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement|Patrol Points", Meta = (MakeEditWidget = true))
		TArray<FVector> PatrollingPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement|Accuracy")
		float MovementAccuracy = 50.f;

	float MovementInterp = 0.f;
	float MovementSmoothnes = 0.f;
	float TempStopFactor = 0.f;
	float TargetForwardAxisValue = 0.f;
	float TargetRightAxisValue = 0.f;
	float CurrentRightAxisValue = 0.f;
	float LastForwardAxisValue = 0.f;
	bool bSwitchCannon = false;

	UPROPERTY()
		ATankPlayerController* TankController;
	UPROPERTY()
		ATankAIController* TankAIController;
	UPROPERTY()
		ACannon* FirstCannon;
	UPROPERTY()
		ACannon* SecondCannon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetViewPosition();

};
