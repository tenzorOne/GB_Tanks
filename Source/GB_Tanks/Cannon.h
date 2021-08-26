// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class UArrowComponent;
class AProjectile;

UCLASS()
class GB_TANKS_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* CannonMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* ProjectileSpawnPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters", meta = (ClampMin = "0"))
		int32 MaxAmmo = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		float FireRate = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		float FireRange = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		float FireDamage = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		ECannonType CannonType = ECannonType::FireProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		TSubclassOf<AProjectile> ProjectileClass;

	FTimerHandle ReloadTimerHandle;
	bool bReadyToFire = true;
	int32 CurrentAmmo;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ECannonType GetCannonType() { return CannonType; };
	void Fire();
	void AutomaticFire();
	void StopAutomaticFire();
	void FireSpecial();
	bool IsReadyToFire();

protected:
	void Reload();

};
