// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class GB_TANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		ATankPawn* TankPawn;
	UPROPERTY()
		FVector MousePosition;
	UPROPERTY()
		FVector MouseDirection;

public:
	FSimpleMulticastDelegate OnMouseButtonUp;

public:
	ATankPlayerController();
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	FVector GetMousePos() { return MousePosition; };

protected:
	virtual void BeginPlay() override;

	void OnLeftMouseButtonUp();
	void MoveForward(float AxisValue);
	void RotateRight(float AxisValue);
	void Fire();
	void StopFire();
	void SwitchCannon();

};
