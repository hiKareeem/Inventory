// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/InvWidgetUtils.h"

int32 UInvWidgetUtils::GetIndexFromSlotPosition(const FIntPoint SlotPosition, const int32 Columns)
{
	return SlotPosition.Y * Columns + SlotPosition.X;
}

FIntPoint UInvWidgetUtils::GetSlotPositionFromIndex(int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}
