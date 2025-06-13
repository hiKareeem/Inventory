// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InvGridSlot.h"

void UInvGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnSlotHovered.Broadcast(SlotIndex, InMouseEvent);
}

void UInvGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	OnSlotUnhovered.Broadcast(SlotIndex, InMouseEvent);
}

FReply UInvGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlotClicked.Broadcast(SlotIndex, InMouseEvent);
	return FReply::Handled();
}

void UInvGridSlot::SetUnoccupiedTexture()
{
	SlotState = EInvGridSlotState::Unoccupied;
	Image_Slot->SetBrush(Brush_Unoccupied);
}

void UInvGridSlot::SetOccupiedTexture()
{
	SlotState = EInvGridSlotState::Occupied;
	Image_Slot->SetBrush(Brush_Occupied);
}

void UInvGridSlot::SetSelectedTexture()
{
	SlotState = EInvGridSlotState::Selected;
	Image_Slot->SetBrush(Brush_Selected);
}

void UInvGridSlot::SetInvalidTexture()
{
	SlotState = EInvGridSlotState::Invalid;
	Image_Slot->SetBrush(Brush_Invalid);
}
