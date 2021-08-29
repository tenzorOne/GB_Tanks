// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "TowerMachineMaster.generated.h"

class ATankPlayerController;
class UStaticMeshCompinent;
class UCameraComponent;
class USpringArmComponent;
class ACannon;

UCLASS()
class GB_TANKS_API ATowerMachineMaster : public APawn
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

	UPROPERTY()
		ACannon* Cannon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void SetupCannon(TSubclassOf<ACannon> CannonClassToSetup);

};
