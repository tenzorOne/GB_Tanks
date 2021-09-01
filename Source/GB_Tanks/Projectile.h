// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Projectile.generated.h"

UCLASS()
class GB_TANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	DECLARE_EVENT(AProjectile, FOnDestroyTarget)

public:	
	// Sets default values for this actor's properties
	AProjectile();

	FOnDestroyTarget OnDestroyTarget;

	void Start();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Movement")
		float MoveSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Movement")
		float MoveRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Movement")
		float FlyRange = 10000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Damage")
		float Damage = 1.f;

	FTimerHandle MovementTimerHandle;
	FDamageData DamageData;

protected:
	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void Move();

};
