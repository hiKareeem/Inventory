// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Spatial/InvInventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagment/InvInventoryComponent.h"
#include "InventoryManagment/InvInventoryUtils.h"
#include "Items/InvItemComponent.h"
#include "Items/Fragments/InvFragmentTags.h"
#include "Items/Fragments/InvItemFragment.h"
#include "Widgets/Inventory/InvGridSlot.h" 
#include "Widgets/Inventory/InvSlottedItem.h"
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

	FInvSlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 1;
	SlotAvailability.Index = 0;
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));
	
	return Result;
}

void UInvInventoryGrid::AddItem(UInvInventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FInvSlotAvailabilityResult Result = HasRoomForItem(Item);
	AddItemToIndices(Result, Item);
}

void UInvInventoryGrid::AddItemToIndices(const FInvSlotAvailabilityResult& Result, UInvInventoryItem* Item)
{
	for (const FInvSlotAvailability& SlotAvailability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(Item, SlotAvailability.Index, Result.bStackable, SlotAvailability.AmountToFill);
	}	
}

UInvSlottedItem* UInvInventoryGrid::CreateSlottedItem(UInvInventoryItem* Item, const int32 Index, const FInvGridFragment* GridFragment, const FInvImageFragment* ImageFragment) const
{
	UInvSlottedItem* SlottedItem = CreateWidget<UInvSlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);
	return SlottedItem; 
}

void UInvInventoryGrid::AddItemAtIndex(UInvInventoryItem* Item, const int32 Index, const bool bStackable,
                                       const int32 StackAmount)
{
	const FInvGridFragment* GridFragment = GetFragment<FInvGridFragment>(Item, FragmentTags::GridFragment);
	const FInvImageFragment* ImageFragment = GetFragment<FInvImageFragment>(Item, FragmentTags::ImageFragment);
	if (!GridFragment || !ImageFragment) return;

	UInvSlottedItem* SlottedItem = CreateSlottedItem(Item, Index, GridFragment, ImageFragment);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	SlottedItems.Add(Index, SlottedItem);
}

void UInvInventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInvGridFragment* GridFragment,
	UInvSlottedItem* SlottedItem) const
{
	CanvasPanel->AddChildToCanvas(SlottedItem);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasPanelSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPosition = UInvWidgetUtils::GetSlotPositionFromIndex(Index, Columns) * SlotSize;
	const FVector2D DrawPositionWithPadding = DrawPosition + FVector2D(GridFragment->GetGridPadding());
	CanvasPanelSlot->SetPosition(DrawPositionWithPadding);
}

void UInvInventoryGrid::SetSlottedItemImage(const UInvSlottedItem* SlottedItem, const FInvGridFragment* GridFragment,
                                            const FInvImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetImage());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImage(Brush);
}

FVector2D UInvInventoryGrid::GetDrawSize(const FInvGridFragment* GridFragment) const
{
	const float IconTileWidth = SlotSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
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
