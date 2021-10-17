// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class UArrowComponent;
class AProjectile;
class UParticleSystemComponent;
class UAudioComponent;
class UMatineeCameraShake;
class AAmmoBox;

UCLASS()
class GB_TANKS_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

	bool bAutomaticFire = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* CannonMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* ProjectileSpawnPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters", meta = (ClampMin = "0"))
		int32 MaxAmmo = 4;
	UPROPERTY(BlueprintReadOnly, Category = "Fire Parameters")
		int32 CurrentAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		float FireRate = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		float FireRange = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters", meta = (EditCondition = "CannonType != ECannonType::FireProjectile"))
		float FireDamage = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		ECannonType CannonType = ECannonType::FireProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters", meta = (EditCondition = "CannonType == ECannonType::FireProjectile"))
		bool bAutomaticCannon = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Parameters")
		TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* ShootEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* AudioEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dynamic Spawn")
		TSubclassOf<AAmmoBox> AmmoBoxForSpawn;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UMatineeCameraShake> ShootShake;

	AActor* CurrentTarget;
	FTimerHandle ReloadTimerHandle;
	FDamageData DamageData;
	bool bReadyToFire = true;

public:	
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	ECannonType GetCannonType() { return CannonType; };
	TSubclassOf<AAmmoBox> GetAmmoBoxForSpawn() { return AmmoBoxForSpawn; };
	void StartFire();
	void StopFire();
	bool IsReadyToFire();
	void AddAmmo(int32 AmmoToAdd);
	void SetCurrentTarget(AActor* InTarget) { CurrentTarget = InTarget; };

protected:
	void Reload();
	void TargetDestroyed();

};
