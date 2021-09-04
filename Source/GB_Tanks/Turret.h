// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "TowerMachineMaster.h"
#include "Turret.generated.h"

class UBoxComponent;
class APawn;

/**
 * 
 */
UCLASS()
class GB_TANKS_API ATurret : public ATowerMachineMaster
{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
 	void Targeting();
 	void RotateToPlayer();
	bool IsPlayerInRange();
	bool DetectPlayerVisibility();
 	bool CanFire();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRate = 0.05f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 10.f;
	UPROPERTY()
		APawn* PlayerPawn;
	
};
