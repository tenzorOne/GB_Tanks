// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "TankFactory.generated.h"

class UBoxComponent;
class UHealthComponent;
class AMapLoader;
class UParticleSystem;

UCLASS()
class GB_TANKS_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankFactory();

	UFUNCTION()
		void TakeDamage(FDamageData& DamageData);
	UFUNCTION()
		void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnNewTank();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
		AMapLoader* LinkedMapLoader;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BuildingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* TankSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tank Spawn Parameters")
		TSubclassOf<ATankPawn> SpawnTankClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tank Spawn Parameters")
		TArray<ATargetPoint*> TankWayPoints;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tank Spawn Parameters")
		float SpawnTankRate = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* OnTankSpawnParticleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* OnHitParticleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* OnDeathParticleEffect;

};
