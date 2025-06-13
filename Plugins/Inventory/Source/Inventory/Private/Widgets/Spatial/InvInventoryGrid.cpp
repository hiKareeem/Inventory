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
#include "Widgets/Inventory/InvHoverItem.h"
#include "Widgets/Inventory/InvSlottedItem.h"
#include "Widgets/Utils/InvWidgetUtils.h"

void UInvInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInvInventoryUtils::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &UInvInventoryGrid::AddItem);
	InventoryComponent->OnStackChanged.AddDynamic(this, &UInvInventoryGrid::AddStacks);
}

FInvSlotAvailabilityResult UInvInventoryGrid::HasRoomForItem(const UInvItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInvSlotAvailabilityResult UInvInventoryGrid::HasRoomForItem(const UInvInventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInvSlotAvailabilityResult UInvInventoryGrid::HasRoomForItem(const FInvItemManifest& Manifest)
{
	FInvSlotAvailabilityResult Result;

	const FInvStackableFragment* StackableFragment = Manifest.GetFragment<FInvStackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	int32 AmountToFill = StackableFragment ? StackableFragment->GetCurrentStack() : 1;
	const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStack() : 1;

	TSet<int32> CheckedIndices;
	for (const TObjectPtr<UInvGridSlot>& GridSlot : GridSlots)
	{
		if (AmountToFill <= 0) break;

		if (CheckedIndices.Contains(GridSlot->GetIndex())) continue;

		if (!IsInGridBounds(GridSlot->GetIndex(), GetDimensions(Manifest))) continue;

		TSet<int32> TentativelyClaimedIndices;
		if (!HasRoomAtIndex(GridSlot, GetDimensions(Manifest), CheckedIndices, TentativelyClaimedIndices, Manifest.GetItemTag(), MaxStackSize)) continue;
		
		const int32 FillAmount = DetermineFillAmountForSlot(Result.bStackable, MaxStackSize, AmountToFill, GridSlot);
		if (FillAmount == 0) continue;

		CheckedIndices.Append(TentativelyClaimedIndices);

		Result.TotalRoomToFill += FillAmount;
		Result.SlotAvailabilities.Emplace(
			FInvSlotAvailability
			{
				HasValidItem(GridSlot) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetIndex(),
				Result.bStackable ? FillAmount : 0,
				HasValidItem(GridSlot)
			}
		);

		AmountToFill -= FillAmount;
		Result.Remainder = AmountToFill;
		if (AmountToFill == 0) return Result;
	}
	return Result;
}

bool UInvInventoryGrid::HasRoomAtIndex(const UInvGridSlot* GridSlot, const FIntPoint& Dimensions,
	const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemTag,
	const int32 MaxStackSize)
{
	bool bHasRoom = true;

	UInvInventoryUtils::ForEach2D(GridSlots, GridSlot->GetIndex(), Dimensions, Columns, [&](const UInvGridSlot* SubGridSlot)
	{
		if (CheckSlotConstraints(GridSlot, SubGridSlot, CheckedIndices, OutTentativelyClaimed, ItemTag, MaxStackSize))
		{
			OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		}
		else
		{
			bHasRoom = false;
		}
	});
	
	return bHasRoom;
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
		UpdateGridSlots(Item, SlotAvailability.Index, Result.bStackable, SlotAvailability.AmountToFill);
	}	
}

UInvSlottedItem* UInvInventoryGrid::CreateSlottedItem(UInvInventoryItem* Item, const int32 Index, const bool bStackable, int32 StackAmount, const FInvGridFragment* GridFragment, const FInvImageFragment* ImageFragment) const
{
	UInvSlottedItem* SlottedItem = CreateWidget<UInvSlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);
	SlottedItem->SetStackable(bStackable);
	SlottedItem->SetStackCount(bStackable ? StackAmount : 0);
	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &UInvInventoryGrid::OnSlottedItemClicked);
	
	return SlottedItem; 
}

void UInvInventoryGrid::AddItemAtIndex(UInvInventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount)
{
	const FInvGridFragment* GridFragment = GetFragment<FInvGridFragment>(Item, FragmentTags::GridFragment);
	const FInvImageFragment* ImageFragment = GetFragment<FInvImageFragment>(Item, FragmentTags::ImageFragment);
	if (!GridFragment || !ImageFragment) return;

	UInvSlottedItem* SlottedItem = CreateSlottedItem(Item, Index, bStackable, StackAmount, GridFragment, ImageFragment);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	SlottedItems.Add(Index, SlottedItem);
}

void UInvInventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInvGridFragment* GridFragment, UInvSlottedItem* SlottedItem) const
{
	CanvasPanel->AddChildToCanvas(SlottedItem);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasPanelSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPosition = UInvWidgetUtils::GetSlotPositionFromIndex(Index, Columns) * SlotSize;
	const FVector2D DrawPositionWithPadding = DrawPosition + FVector2D(GridFragment->GetGridPadding());
	CanvasPanelSlot->SetPosition(DrawPositionWithPadding);
}

void UInvInventoryGrid::UpdateGridSlots(UInvInventoryItem* Item, const int32 Index, bool bStackable, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackable)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}
 
	const FInvGridFragment* GridFragment = GetFragment<FInvGridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return;

	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UInvInventoryUtils::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInvGridSlot* GridSlot)
	{
		GridSlot->SetItem(Item);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetOccupiedTexture();
		GridSlot->SetAvailable(false);
	});
}

FIntPoint UInvInventoryGrid::GetDimensions(const FInvItemManifest& Manifest) const
{
	const FInvGridFragment* GridFragment = Manifest.GetFragment<FInvGridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
}

bool UInvInventoryGrid::CheckSlotConstraints(const UInvGridSlot* GridSlot, const UInvGridSlot* SubGridSlot, const TSet<int32>& CheckedIndices, TSet<int32>& TentativelyClaimedIndices, const FGameplayTag& ItemTag, const int32 MaxStackSize) const
{
	if (CheckedIndices.Contains(SubGridSlot->GetIndex())) return false;
	if (!HasValidItem(SubGridSlot))
	{
		TentativelyClaimedIndices.Add(SubGridSlot->GetIndex());
		return true;
	}
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false; 

	const UInvInventoryItem* SubItem = SubGridSlot->GetItem().Get();
	if (!SubItem->IsStackable()) return false;

	if (!DoesItemTypeMatch(SubItem, ItemTag)) return false;

	if (GridSlot->GetStackCount() >= MaxStackSize) return false;
	
	return true;
}

bool UInvInventoryGrid::HasValidItem(const UInvGridSlot* GridSlot) const
{
	return GridSlot->GetItem().IsValid();
}

bool UInvInventoryGrid::IsUpperLeftSlot(const UInvGridSlot* GridSlot, const UInvGridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetIndex();
}

bool UInvInventoryGrid::DoesItemTypeMatch(const UInvInventoryItem* Item, const FGameplayTag& ItemTag) const
{
	return Item->GetItemManifest().GetItemTag().MatchesTagExact(ItemTag);
}

bool UInvInventoryGrid::IsInGridBounds(const int32 StartingIndex, const FIntPoint& Dimensions) const
{
	if (StartingIndex < 0 || StartingIndex >= GridSlots.Num()) return false;
	const int32 EndColumn = (StartingIndex % Columns) + Dimensions.X;
	const int32 EndRow = (StartingIndex / Columns) + Dimensions.Y;
	return EndColumn <= Columns || EndRow <= Rows;
}

int32 UInvInventoryGrid::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInvGridSlot* GridSlot) const
{
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 UInvInventoryGrid::GetStackAmount(const UInvGridSlot* GridSlot) const
{
	int32 CurrentStackAmount = GridSlot->GetStackCount();

	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex(); UpperLeftIndex != INDEX_NONE)
	{
		UInvGridSlot* UpperLeftSlot = GridSlots[UpperLeftIndex];
		CurrentStackAmount = UpperLeftSlot->GetStackCount();
	}

	return CurrentStackAmount;
}

void UInvInventoryGrid::AddStacks(const FInvSlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const FInvSlotAvailability& SlotAvailability : Result.SlotAvailabilities)
	{
		if (SlotAvailability.bItemAtIndex)
		{
			const TObjectPtr<UInvGridSlot>& GridSlot = GridSlots[SlotAvailability.Index];
			const TObjectPtr<UInvSlottedItem>& SlottedItem = SlottedItems.FindChecked(SlotAvailability.Index);
			SlottedItem->SetStackCount(GridSlot->GetStackCount() + SlotAvailability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + SlotAvailability.AmountToFill);
		}
		else
		{
			AddItemAtIndex(Result.Item.Get(), SlotAvailability.Index, Result.bStackable, SlotAvailability.AmountToFill);
			UpdateGridSlots(Result.Item.Get(), SlotAvailability.Index, Result.bStackable, SlotAvailability.AmountToFill);
		}
	}
}

void UInvInventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UInvInventoryItem* ClickedItem = GridSlots[GridIndex]->GetItem().Get();

	if (!IsValid(HoveredItem) && IsLeftClick(MouseEvent))
	{
		PickUpItem(ClickedItem, GridIndex);
	}
}

bool UInvInventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UInvInventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UInvInventoryGrid::PickUpItem(UInvInventoryItem* ClickedItem, const int32 GridIndex)
{
	AssignHoveredItem(ClickedItem, GridIndex, GridIndex);

	RemoveItemFromGrid(ClickedItem, GridIndex);
}

void UInvInventoryGrid::AssignHoveredItem(UInvInventoryItem* ClickedItem)
{
	if (!IsValid(HoveredItem))
	{
		HoveredItem = CreateWidget<UInvHoverItem>(GetOwningPlayer(), HoveredItemClass);
	}

	const FInvGridFragment* GridFragment = GetFragment<FInvGridFragment>(ClickedItem, FragmentTags::GridFragment);
	const FInvImageFragment* ImageFragment = GetFragment<FInvImageFragment>(ClickedItem, FragmentTags::ImageFragment);
	if (!GridFragment || !ImageFragment) return;

	const FVector2D DrawSize = GetDrawSize(GridFragment);

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetImage());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);
	
	HoveredItem->SetImageBrush(Brush);
	HoveredItem->SetGridDimensions(GridFragment->GetGridSize());
	HoveredItem->SetItem(ClickedItem);
	HoveredItem->SetStackable(ClickedItem->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoveredItem);
}

void UInvInventoryGrid::AssignHoveredItem(UInvInventoryItem* ClickedItem, const int32 GridIndex,
	const int32 PreviousGridIndex)
{
	AssignHoveredItem(ClickedItem);

	HoveredItem->SetPreviousIndex(PreviousGridIndex);
	HoveredItem->UpdateStackCount(ClickedItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UInvInventoryGrid::RemoveItemFromGrid(UInvInventoryItem* ClickedItem, const int32 GridIndex)
{
	const FInvGridFragment* GridFragment = GetFragment<FInvGridFragment>(ClickedItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	UInvInventoryUtils::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&](UInvGridSlot* GridSlot)
	{
		GridSlot->SetItem(nullptr);
		GridSlot->SetUpperLeftIndex(INDEX_NONE);
		GridSlot->SetUnoccupiedTexture();
		GridSlot->SetAvailable(true);
		GridSlot->SetStackCount(0);
	});

	if (SlottedItems.Contains(GridIndex))
	{
		TObjectPtr<UInvSlottedItem> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
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
