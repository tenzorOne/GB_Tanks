// Copyright Epic Games, Inc. All Rights Reserved.


#include "GB_TanksGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "TimerManager.h"

// NormalizedPosition - ��� �������� (static), ������� ����� ������������ � �����
FVector2D AGB_TanksGameModeBase::NormalizedPosition = FVector2D(0.f);

// ��� ������ ���� � ����� ����� ������, ������� �������� ����� � ������ "BOUND"
// ���� ����� ����� ������ (� � ���� ������ ��� ��� ���� (SM_Floor) ����� ������), �� �� ������� �������, ������� ������ ��� ������� ������ �� ���� ������ � �������� ����������� ������ �� ��������� ��������� �������
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

// �����, ������� ������ ������� ������, ���� � ���� ���� ���������� ���
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

// �����, ����������� ������� ������ � ������������ �� �������
void AGB_TanksGameModeBase::UpdatePosition()
{
	NormalizedPosition = GetActorPositionInBounds();

}

// ������� �����: ����� ������� ������, �������� BoundingBox ������ � �����, ����� ���� ������������� ������� ������ � ���� ����� �����������/������������ ������ ����� BoundingBox'�
// �.�. ���������� ����� ����� �����������, ������� ������ ����� �������� �� ������ ������, �� ����� ������������� ��� � ���������� ����� 0 � 1, � ����� ������ ��� � �����, ����� �������� ������� ����� �������� �������
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
