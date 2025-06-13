// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/InvWidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Widget.h"

int32 UInvWidgetUtils::GetIndexFromSlotPosition(const FIntPoint SlotPosition, const int32 Columns)
{
	return SlotPosition.Y * Columns + SlotPosition.X;
}

FIntPoint UInvWidgetUtils::GetSlotPositionFromIndex(int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

FVector2D UInvWidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry& Geometry = Widget->GetCachedGeometry();

	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition);

	return ViewportPosition;
}

bool UInvWidgetUtils::IsWithinBounds(const FVector2D& BoundaryPosition, const FVector2D& WidgetSize,
	const FVector2D& MousePosition)
{
	return MousePosition.X >= BoundaryPosition.X && MousePosition.X <= BoundaryPosition.X + WidgetSize.X &&
		MousePosition.Y >= BoundaryPosition.Y && MousePosition.Y <= BoundaryPosition.Y + WidgetSize.Y;
}

FVector2D UInvWidgetUtils::GetWidgetSize(const UWidget* Widget)
{
	const FGeometry& Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}
