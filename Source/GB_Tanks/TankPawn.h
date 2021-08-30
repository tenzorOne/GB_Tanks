// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshCompinent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;

UCLASS()
class GB_TANKS_API ATankPawn : public APawn
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
		void Fire();
	UFUNCTION()
		void StopAutomaticFire();
	UFUNCTION()
		void FireSpecial();
	UFUNCTION()
		float GetDefaultStopFactor() { TempStopFactor = StopInertiaFactor; return TempStopFactor; };
	UFUNCTION()
		void SetupCannon(ACannon* CurrentCannon, TSubclassOf<ACannon> InCannonClass);
	UFUNCTION()
		void InstallNewCannon(TSubclassOf<ACannon> InCannonClass);
	UFUNCTION()
		void ChangeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* CannonSetupPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> MainCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> SecondCannonClass;

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

	float MovementInterp = 0.f;
	float MovementSmoothnes = 0.f;
	float TempStopFactor;
	float TargetForwardAxisValue = 0.f;
	float TargetRightAxisValue = 0.f;
	float CurrentRightAxisValue = 0.f;
	float LastForwardAxisValue = 0.f;
	bool bIsMainCannon = true;

	UPROPERTY()
		ATankPlayerController* TankController;
	UPROPERTY()
		ACannon* MainCannon;
	UPROPERTY()
		ACannon* SecondCannon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY()
	ACannon* CurrentCannon;

};
