// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include <Components/SphereComponent.h>
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
 	void RotateToTarget();
	bool TargetInRange();
	bool DetectTargetVisibility();
 	bool CanFire();

protected:
	UFUNCTION()
		void BeginComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USphereComponent* TargetsCheckVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Logic")
		bool bPlayersAlly = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRate = 0.05f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 10.f;

protected:
	APawn* PlayerPawn;
	AActor* CurrentTarget;
	
};
