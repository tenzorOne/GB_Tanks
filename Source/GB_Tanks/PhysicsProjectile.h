// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsProjectile.generated.h"

class UPhysicalTrajectoryComponent;

/**
 * 
 */
UCLASS()
class GB_TANKS_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UPhysicalTrajectoryComponent* PhysicsComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* TrailEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MoveAccurency = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationMaxTime = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationTimeStep = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationSpeed = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float ExplodeRadius = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		bool bShowTrajectory = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement Parameters")
		FVector MoveVector;
	UPROPERTY(BlueprintReadWrite, Category = "Movement Parameters")
		TArray<FVector> CurrentTrajectory;
	UPROPERTY(BlueprintReadWrite, Category = "Movement Parameters")
		int32 TragectoryPointIndex;

public:
	APhysicsProjectile();

	virtual void Start() override;

protected:
	virtual void Move() override;
	void Explode();

};
