// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GB_TANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(UHealthComponent, FOnDie, AActor*)

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	FOnDie OnDie;

	float GetHealth() const;
	float GetHealthState() const;
	void TakeDamage(FDamageData DamageData);
	void AddHealth(float AddiditionalHealthValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Parameters")
		float MaxHealth = 10.f;
	UPROPERTY()
		float CurrentHealth;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
