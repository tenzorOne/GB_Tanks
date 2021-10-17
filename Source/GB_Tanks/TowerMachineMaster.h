// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "IScorable.h"
#include "TowerMachineMaster.generated.h"

class UHealthComponent;
class UStaticMeshCompinent;
class ACannon;
class UParticleSystem;

DECLARE_DELEGATE(FTakeDamage)

UCLASS()
class GB_TANKS_API ATowerMachineMaster : public APawn, public IDamageTaker, public IIScorable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATowerMachineMaster();

	UFUNCTION()
		void SetupCannon(TSubclassOf<ACannon> CannonClassToSetup);
	UFUNCTION(BlueprintCallable)
		ACannon* GetCurrentCannon() { return Cannon != nullptr ? Cannon : nullptr; };
	UFUNCTION()
		void RotateTurretTo(FVector TargetPosition);
	UFUNCTION()
		void StartFire();
	UFUNCTION()
		void StartFire_WithCurrentTarget(AActor* CurrentTarget);
	UFUNCTION()
		void StopFire();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Take Damage"))
		void EventTakeDamage();
	UFUNCTION()
		virtual void TakeDamage(FDamageData& DamageData) override;
	UFUNCTION()
		virtual void EarningPoints() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* CannonSetupPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon", meta = (DisplayPriority = "1"))
		TSubclassOf<ACannon> CannonClass = ACannon::StaticClass();
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Score Points")
		int32 CurrentPoints = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Rotation")
		bool bRotationByCannonPosition = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Rotation")
		bool bUseTurretConstantInterpRotation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Rotation")
		float TurretRotationSpeed = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* OnHitParticleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* OnDeathParticleEffect;

	UPROPERTY()
		ACannon* Cannon;

	FTakeDamage TakeDamageDelegate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetViewPosition();
	
	virtual void Die();

};
