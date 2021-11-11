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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float MoveAccurency = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float SimStep = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float MaxSimTime = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		bool bShowTrajectory = false;

	UPROPERTY(BlueprintReadWrite, Category = "Movement Parameters")
		TArray<FVector> CurrentTrajectory;
	UPROPERTY(BlueprintReadWrite, Category = "Movement Parameters")
		int32 TrajectoryPointIndex;

public:
	APhysicsProjectile();

	virtual void Start() override;
	UPhysicalTrajectoryComponent* GetPhysicalComponent() { return PhysicsComponent; };

protected:
	virtual void Move() override;

};
