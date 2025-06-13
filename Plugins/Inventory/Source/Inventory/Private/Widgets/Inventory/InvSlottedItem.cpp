// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/InvSlottedItem.h"

#include "Components/TextBlock.h"
#include "Items/InvInventoryItem.h"

FReply UInvSlottedItem::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex, MouseEvent);
	return FReply::Handled();
}

void UInvSlottedItem::SetItem(UInvInventoryItem* NewItem)
{
	Item = NewItem;
}

void UInvSlottedItem::SetStackCount(const int32 Count)
{
	if (Count > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(Count));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
