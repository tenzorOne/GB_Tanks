// Fill out your copyright notice in the Description page of Project Settings.


#include "SMiniMap.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "SlateOptMacros.h"

void SMiniMap::Construct(const FArguments& InArgs)
{		
	// изначальная инициализация PlayerPos сделана из аргументов Слейта, как и показано в методичке/на вебинаре
	// но как обновить эту переменную далее без самопальных функций, я так и не понял
	PlayerPos = InArgs._PlayerPosition;

	// заводим таймер-делегат и вешаем на него вызов функции
	FWidgetActiveTimerDelegate TimerDelegate;
	TimerDelegate.BindRaw(this, &SMiniMap::RePaint);

	// регистрируем таймер в Слейте, который будет срабатывать каждые 0.01 секунды
	SWidget::RegisterActiveTimer(0.01f, TimerDelegate);

}

int32 SMiniMap::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{		
	FVector2D LocalSize = AllottedGeometry.GetLocalSize();
	FSlateBrush MiniMapBrush;

	// рисуем серую подложку под мини-карту
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		&MiniMapBrush,
		ESlateDrawEffect::NoPixelSnapping,
		FLinearColor(0.05f, 0.05f, 0.05f, 1.f)
	);

	// рисуем границы для мини-карты
	TArray<FVector2D> MapBorder;
	MapBorder.Add(LocalSize * FVector2D(0.f, 0.f));
	MapBorder.Add(LocalSize * FVector2D(0.f, 1.f));
	MapBorder.Add(LocalSize * FVector2D(1.f, 1.f));
	MapBorder.Add(LocalSize * FVector2D(1.f, 0.f));
	MapBorder.Add(LocalSize * FVector2D(0.f, 0.f));
	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		MapBorder,
		ESlateDrawEffect::NoPixelSnapping,
		FLinearColor::White,
		true,
		5.f
	);

	// фабрика танков, которая стоит в центре уровня
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(FSlateLayoutTransform(0.15f, FVector2D(85.f, 85.f))),
		&MiniMapBrush,
		ESlateDrawEffect::NoPixelSnapping,
		FLinearColor::Red
	);

	// рисуем препятсвия на мини-карте
	TArray<FBox2D> Obstacles;
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.18f, 0.4f), LocalSize * FVector2D(0.18f, 0.8f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.18f, 0.52f), LocalSize * FVector2D(0.08f, 0.52f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.f, 0.7f), LocalSize * FVector2D(0.08f, 0.7f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.12f, 0.8f), LocalSize * FVector2D(0.35f, 0.8f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.28f, 0.5f), LocalSize * FVector2D(0.28f, 0.6f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.6f, 0.5f), LocalSize * FVector2D(0.82f, 0.5f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.18f, 0.5f), LocalSize * FVector2D(0.4f, 0.5f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.76f, 0.46f), LocalSize * FVector2D(0.76f, 0.7f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.6f, 0.7f), LocalSize * FVector2D(0.88f, 0.7f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.26f, 0.35f), LocalSize * FVector2D(0.33f, 0.35f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.8f, 0.24f), LocalSize * FVector2D(0.87f, 0.31f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.75f, 0.f), LocalSize * FVector2D(0.75f, 0.15f)));
	Obstacles.Add(FBox2D(LocalSize * FVector2D(0.6f, 0.5f), LocalSize * FVector2D(0.6f, 0.f)));

	for (auto& CurrObstacle : Obstacles)
	{
		TArray<FVector2D> Points;
		Points.Add(CurrObstacle.Min);
		Points.Add(CurrObstacle.Max);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::NoPixelSnapping,
			FLinearColor::White,
			true,
			2.f
		);
	}

	// игрок на мини-карте
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(FSlateLayoutTransform(0.05f, LocalSize * PlayerPos)),
		&MiniMapBrush,
		ESlateDrawEffect::None,
		FLinearColor::Green
	);

	return 0;

}

// функция вызывается по таймеру (выше) и обновляет PlayerPos через вызов функции в ГеймМоде
// еще я вызываю SWidget::Invalidate для гарантии того, что закэшированный виджет будет признан не действительным и отправлен на перерисовку в следующем кадре
EActiveTimerReturnType SMiniMap::RePaint(double InCurrentTime, float InDeltaTime)
{
	PlayerPos = AGB_TanksGameModeBase::GetNormalizedPosition();
	SWidget::Invalidate(EInvalidateWidgetReason::Paint);

	return EActiveTimerReturnType::Continue;

}
