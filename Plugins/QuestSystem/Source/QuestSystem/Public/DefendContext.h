// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DefendContext.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDefendContext : public UInterface
{
	GENERATED_BODY()
};

/*
 *
 * Вся основная логика в этот раз реализована в BP, в плюсах мы имеем лишь подготовку почвы к следующему ДЗ и реализации уже квестовой системы непосредственно.
 * Тем не менее для задания со звездочкой я решил создать каркас "цели" квеста, которая подразумевает защиту/захват какого-либо объекта (это может быть что угодно - зона для захвата, замок в осаде, NPC, которого мы должны защитить).
 * Делегаты, объявленные ниже, позволют конструировать логику в игре исходя из контекста:
 * - при начале защиты/захвата объекта воспользуемся OnStartDefend;
 * - если мы успешно завершили защиту/захват объекта, то воспользуемся OnCompletedDefend;
 * - если по каким-то причинам мы прервали защиту/захват объекта (вышли из зоны захвата, к примеру), то воспользуемся OnInterruptDefend (содержит доп. информацию о выполненных процентах защиты/захвата);
 * - если цель можно провалить и мы не смогли защитить/захватить объект, то воспользуемся OnFailedDefend;
 * 
*/

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCompletedDefend, AActor* /*DefenseStartedActor*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInterruptDefend, AActor* /*DefenseStartedActor*/, int32 /*PercentageBeforeCompleted*/);
DECLARE_MULTICAST_DELEGATE(FOnStartDefend);
DECLARE_MULTICAST_DELEGATE(FOnFailedDefend);

/**
 * 
 */
class QUESTSYSTEM_API IDefendContext
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void NotifyDefendCompleted(AActor* DefenseStartedActor);
	virtual void NotifyDefendInterrupted(AActor* DefenseStartedActor, int32 PercentageBeforeCompleted);
	virtual void NotifyDefendStarted();
	virtual void NotifyDefendFailed();

	FOnCompletedDefend OnCompletedDefend;
	FOnInterruptDefend OnInterruptDefend;
	FOnStartDefend OnStartDefend;
	FOnFailedDefend OnFailedDefend;
	
};

UCLASS()
class QUESTSYSTEM_API ADefendContextActor : public AActor
{
	GENERATED_BODY()
    
	UFUNCTION(BlueprintCallable)
	void OnDefendCompleted(AActor* ActorWhoStartedDefense)
	{
		if (IDefendContext* DefendContext = Cast<IDefendContext>(this))
		{
			DefendContext->NotifyDefendCompleted(ActorWhoStartedDefense);
		}
	}

	UFUNCTION(BlueprintCallable)
	void OnDefendInterrupted(AActor* ActorWhoStartedDefense, int32 PercentageBeforeCompleted)
	{
		if (IDefendContext* DefendContext = Cast<IDefendContext>(this))
		{
			DefendContext->NotifyDefendInterrupted(ActorWhoStartedDefense, PercentageBeforeCompleted);
		}
	}

	UFUNCTION(BlueprintCallable)
	void OnDefendStarted()
	{
		if (IDefendContext* DefendContext = Cast<IDefendContext>(this))
		{
			DefendContext->NotifyDefendStarted();
		}
	}
	
	UFUNCTION(BlueprintCallable)
	void OnDefendFailed()
	{
		if (IDefendContext* DefendContext = Cast<IDefendContext>(this))
		{
			DefendContext->NotifyDefendFailed();
		}
	}
	
};