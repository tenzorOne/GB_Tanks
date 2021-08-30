// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "DamageTaker.h"
#include "TowerMachineMaster.generated.h"

class ATankPlayerController;
class UStaticMeshCompinent;
class UCameraComponent;
class USpringArmComponent;
class ACannon;

UCLASS()
class GB_TANKS_API ATowerMachineMaster : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATowerMachineMaster();

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

	UPROPERTY()
		ACannon* Cannon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Die();

	UFUNCTION()
		virtual void TakeDamage(FDamageData DamageData) override;
	UFUNCTION()
		void SetupCannon(TSubclassOf<ACannon> CannonClassToSetup);

};
