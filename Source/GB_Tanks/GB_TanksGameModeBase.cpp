// Copyright Epic Games, Inc. All Rights Reserved.


#include "GB_TanksGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "TimerManager.h"

// NormalizedPosition - это значение (static), которое будет передаватьс€ в —лейт
FVector2D AGB_TanksGameModeBase::NormalizedPosition = FVector2D(0.f);

// при начале игры € прошу найти актора, который обаладет тэгом с именем "BOUND"
// если такой актор найден (а в моем случае это меш ѕола (SM_Floor) всего уровн€), то мы вызовем функцию, котора€ найдет нам позицию игрока на этом акторе и запустит рекурсивный таймер на следующую обработку позиции
void AGB_TanksGameModeBase::BeginPlay()
{
	ActorWithTag = GetActorByBoundTag("BOUND");
	if (ActorWithTag)
	{
		FTimerHandle PositionUpdateHandle;
		GetWorld()->GetTimerManager().SetTimer(PositionUpdateHandle, this, &AGB_TanksGameModeBase::UpdatePosition, 0.1f, true);
		
		NormalizedPosition = GetActorPositionInBounds();
	}
	else
	{
		NormalizedPosition = FVector2D(0.f, 0.f);
	}

}

// метод, который вернет нужного актора, если у него есть конкретный тэг
AActor* AGB_TanksGameModeBase::GetActorByBoundTag(FName TagName)
{
	TArray<AActor*> ActorsMeanBounds;
	
	UGameplayStatics::GetAllActorsWithTag(this, TagName, ActorsMeanBounds);

	if (ActorsMeanBounds.Num())
	{
		return ActorsMeanBounds.Pop();
	}
	else
	{
		return nullptr;
	}

}

// метод, обновл€ющий позицию игрока и вызывающийс€ по таймеру
void AGB_TanksGameModeBase::UpdatePosition()
{
	NormalizedPosition = GetActorPositionInBounds();

}

// принцип прост: вз€ть позицию игрока, получить BoundingBox актора с тэгом, после чего нормализовать позицию игрока в мире через минимальный/максимальный размер этого BoundingBox'а
// т.к. полученное число будет рассто€нием, которое вообще можно проехать на данном уровне, мы можем нормализовать его в промежутке между 0 и 1, а потом подать это в —лейт, чтобы задавать позицию более удобными числами
FVector2D AGB_TanksGameModeBase::GetActorPositionInBounds()
{
	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector ActorBounds;
	FVector ActorOrigin;
	
	ActorWithTag->GetActorBounds(false, ActorOrigin, ActorBounds, false);

	float PlayerXPos;
	float PlayerYPos;
	float PlayerZPos;
	UKismetMathLibrary::BreakVector(PlayerPosition, PlayerXPos, PlayerYPos, PlayerZPos);
	float ActorXBound;
	float ActorYBound;
	float ActorZBound;
	UKismetMathLibrary::BreakVector(ActorBounds, ActorXBound, ActorYBound, ActorZBound);

	float XNormalizePos = UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizeToRange(PlayerYPos, ActorXBound * -1.f, ActorXBound));
	float YNormalizePos = UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizeToRange(PlayerXPos * -1.f, ActorYBound * -1.f, ActorYBound));

	FVector2D Position = UKismetMathLibrary::MakeVector2D(XNormalizePos, YNormalizePos);
	return Position;

}
