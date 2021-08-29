// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "TowerMachineMaster.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Turret.generated.h"

class UBoxComponent;
class APawn;

/**
 * 
 */
UCLASS()
class GB_TANKS_API ATurret : public ATowerMachineMaster, public IDamageTaker
{
	GENERATED_BODY()

public:
	ATurret();

	UFUNCTION()
		virtual void TakeDamage(FDamageData DamageData) override;
	UFUNCTION()
		void DamageTaked(float DamageValue);
	UFUNCTION()
		void Die();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
 	void Targeting();
 	void RotateToPlayer();
 	bool IsPlayerInRange();
 	bool CanFire();
 	void Fire();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 10;
	UPROPERTY()
		APawn* PlayerPawn;
	
};
