// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	// ...
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::TakeDamage(FDamageData& DamageData)
{
	float TakedDamageValue = DamageData.DamageValue;
	AActor* DamageMaker = DamageData.DamageMaker;
	CurrentHealth -= TakedDamageValue;

	if (CurrentHealth <= 0)
	{
		if (OnDie.IsBound())
		{
			DamageData.bTargetKilled = true;
			OnDie.Broadcast();
		}
	}

}

void UHealthComponent::AddHealth(float AddiditionalHealthValue)
{
	CurrentHealth += AddiditionalHealthValue;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
		
}