// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "IScorable.h"
#include "TowerMachineMaster.generated.h"

class UHealthComponent;
class UStaticMeshCompinent;
class ACannon;

UCLASS()
class GB_TANKS_API ATowerMachineMaster : public APawn, public IDamageTaker, public IIScorable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATowerMachineMaster();

	UFUNCTION()
		void SetupCannon(TSubclassOf<ACannon> CannonClassToSetup);
	UFUNCTION()
		ACannon* GetCurrentCannon() { return Cannon; };
	UFUNCTION()
		void StartFire();
	UFUNCTION()
		void StopFire();
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

	UPROPERTY()
		ACannon* Cannon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Die(AActor* DamageMaker);

};
