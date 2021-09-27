// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Projectile.generated.h"

class UParticleSystem;
class IDamageTaker;

UCLASS()
class GB_TANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	DECLARE_EVENT(AProjectile, FOnDestroyTarget)

public:	
	// Sets default values for this actor's properties
	AProjectile();

	FOnDestroyTarget OnDestroyTarget;

	virtual void Start();

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
		bool bExplosiveProjectile = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Damage", meta = (EditCondition = "bExplosiveProjectile", EditConditionHides))
		float ExplodeRadius = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Damage")
		float Damage = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Damage")
		float PushForce = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystem* OnDeathParticleEffect;

	FTimerHandle MovementTimerHandle;
	FDamageData DamageData;

protected:
	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void Move();
	UFUNCTION()
		void Explode();
		
	void DamageActor(IDamageTaker* DamageTakerActor);
	void PushActor(UPrimitiveComponent* Component, FVector PushLocation, float InPushForce);

};
