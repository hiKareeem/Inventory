// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Spatial/InvInventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagment/InvInventoryComponent.h"
#include "InventoryManagment/InvInventoryUtils.h"
#include "Items/InvItemComponent.h"
#include "Widgets/Inventory/InvGridSlot.h" 
#include "Widgets/Utils/InvWidgetUtils.h"

void UInvInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInvInventoryUtils::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &UInvInventoryGrid::AddItem);
}

FInvSlotAvailabilityResult UInvInventoryGrid::HasRoomForItem(const UInvItemComponent* ItemComponent) const
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInvSlotAvailabilityResult UInvInventoryGrid::HasRoomForItem(const UInvInventoryItem* Item) const
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInvSlotAvailabilityResult UInvInventoryGrid::HasRoomForItem(const FInvItemManifest& Manifest) const
{
	FInvSlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;
	return Result;
}

void UInvInventoryGrid::AddItem(UInvInventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FInvSlotAvailabilityResult Result = HasRoomForItem(Item);
}

void UInvInventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int32 Row = 0; Row < Rows; Row++)
	{
		for (int32 Column = 0; Column < Columns; Column++)
		{
			UInvGridSlot* GridSlot = CreateWidget<UInvGridSlot>(this, GridSlotClass);
			CanvasPanel->AddChildToCanvas(GridSlot);

			FIntPoint SlotPosition(Column, Row);
			GridSlot->SetIndex(UInvWidgetUtils::GetIndexFromSlotPosition(SlotPosition, Columns));

			UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			CanvasSlot->SetSize(FVector2D(SlotSize));
			CanvasSlot->SetPosition(SlotPosition * SlotSize);
			
			GridSlots.Add(GridSlot);
		}
	}
}
