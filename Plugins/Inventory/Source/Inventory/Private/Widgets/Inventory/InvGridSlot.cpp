// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InvGridSlot.h"

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
