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

void UInvInventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPosition = UInvWidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPosition, UInvWidgetUtils::GetWidgetSize(CanvasPanel), MousePosition))
	{
		return;
	}
	
	UpdateTileParameters(CanvasPosition, MousePosition);
}

bool UInvInventoryGrid::CursorExitedCanvas(const FVector2D& BoundaryPosition, const FVector2D& BoundarySize,
	const FVector2D& Location)
{
	bLastMouseWithinCanvas = bMouseWithinCanvas;
	bMouseWithinCanvas = UInvWidgetUtils::IsWithinBounds(BoundaryPosition, BoundarySize, Location);

	if (!bMouseWithinCanvas && bLastMouseWithinCanvas)
	{
		UnhighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
		return true; 
	}

	return false;
}

void UInvInventoryGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	if (!bMouseWithinCanvas) return;

	UnhighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	
	UInvInventoryUtils::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInvGridSlot* GridSlot)
	{
		GridSlot->SetOccupiedTexture();
	});
	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

void UInvInventoryGrid::UnhighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	UInvInventoryUtils::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInvGridSlot* GridSlot)
	{
		if (GridSlot->IsAvailable())
		{
			GridSlot->SetUnoccupiedTexture();
		}
		else
		{
			GridSlot->SetOccupiedTexture();
		}
	});
}

void UInvInventoryGrid::ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EInvGridSlotState GridSlotState)
{
	UnhighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	
	UInvInventoryUtils::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInvGridSlot* GridSlot)
	{
		switch (GridSlotState)
		{
			case EInvGridSlotState::Occupied:
				GridSlot->SetOccupiedTexture();
				break;
			case EInvGridSlotState::Selected:
				GridSlot->SetSelectedTexture();
				break;
			case EInvGridSlotState::Invalid:
				GridSlot->SetInvalidTexture();
				break;
			case EInvGridSlotState::Unoccupied:
				GridSlot->SetUnoccupiedTexture();
				break;
			default:
				break;
		}
	});
	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

void UInvInventoryGrid::UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	if (!bMouseWithinCanvas) return;
	
	const FIntPoint HoveredCoordinates = CalculateHoveredCoordinates(CanvasPosition, MousePosition);
	LastTileParameters = TileParameters;
	TileParameters.TileCoordinates = HoveredCoordinates;
	TileParameters.TileIndex = UInvWidgetUtils::GetIndexFromSlotPosition(HoveredCoordinates, Columns);
	TileParameters.Quadrant = DetermineHoveredQuadrant(CanvasPosition, MousePosition);

	OnTileParametersChanged(TileParameters);
}

void UInvInventoryGrid::OnTileParametersChanged(const FInvTileParameters& InTileParameters)
{
	if (!IsValid(HoveredItem)) return;

	const FIntPoint Dimensions = HoveredItem->GetGridDimensions();
	const FIntPoint StartingCoordinates = CalculateStartingCoordinates(InTileParameters.TileCoordinates, Dimensions, InTileParameters.Quadrant);
	ItemDropIndex = UInvWidgetUtils::GetIndexFromSlotPosition(StartingCoordinates, Columns);

	CurrentSpaceQueryResult = CheckHoverPosition(StartingCoordinates, Dimensions);

	if (CurrentSpaceQueryResult.bHasSpace)
	{
		HighlightSlots(ItemDropIndex, Dimensions);
		return;
	}
	UnhighlightSlots(ItemDropIndex, LastHighlightedDimensions);

	if (CurrentSpaceQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQueryResult.UpperLeftIndex))
	{
		const FInvGridFragment* GridFragment = GetFragment<FInvGridFragment>(CurrentSpaceQueryResult.ValidItem.Get(), FragmentTags::GridFragment);
		if (!GridFragment) return;

		ChangeHoverType(CurrentSpaceQueryResult.UpperLeftIndex, GridFragment->GetGridSize(), EInvGridSlotState::Invalid);
	}
}

FInvSpaceQueryResult UInvInventoryGrid::CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions)
{
	FInvSpaceQueryResult Result;

	if (!IsInGridBounds(UInvWidgetUtils::GetIndexFromSlotPosition(Position, Columns), Dimensions)) return Result;
	
	Result.bHasSpace = true;

	TSet<int32> OccupiedUpperLeftIndices;
	UInvInventoryUtils::ForEach2D(GridSlots, UInvWidgetUtils::GetIndexFromSlotPosition(Position, Columns), Dimensions, Columns,
		[&](UInvGridSlot* GridSlot)
			{
				if (GridSlot->GetItem().IsValid())
				{
					OccupiedUpperLeftIndices.Add(GridSlot->GetUpperLeftIndex());
					Result.bHasSpace = false;
				}
			});

	if (OccupiedUpperLeftIndices.Num() == 1)
	{
		const int32 OccupiedUpperLeftIndex = *OccupiedUpperLeftIndices.CreateConstIterator();
		Result.ValidItem = GridSlots[OccupiedUpperLeftIndex]->GetItem();
		Result.UpperLeftIndex = OccupiedUpperLeftIndex;
	}
	
	return Result;
}

FIntPoint UInvInventoryGrid::CalculateStartingCoordinates(const FIntPoint& Coordinates, const FIntPoint& Dimensions,
                                                          const EInvTileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimensions.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoordinates;

	switch (Quadrant)
	{
		case EInvTileQuadrant::TopLeft:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
			break;
		case EInvTileQuadrant::TopRight:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
			break;
		case EInvTileQuadrant::BottomLeft:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
			break;
		case EInvTileQuadrant::BottomRight:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
			break;
		default:
			return FIntPoint(-1, -1);
	}

	return StartingCoordinates; 
}

FIntPoint UInvInventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPosition,
                                                         const FVector2D& MousePosition) const
{
	return FIntPoint(
		static_cast<int32>(FMath::Floor((MousePosition.X - CanvasPosition.X) / SlotSize)),
		static_cast<int32>(FMath::Floor((MousePosition.Y - CanvasPosition.Y) / SlotSize))
	);
}

EInvTileQuadrant UInvInventoryGrid::DetermineHoveredQuadrant(const FVector2D& CanvasPosition,
	const FVector2D& MousePosition) const
{
	const float TileLocalX = FMath::Fmod(MousePosition.X - CanvasPosition.X, SlotSize);
	const float TileLocalY = FMath::Fmod(MousePosition.Y - CanvasPosition.Y, SlotSize);

	if (TileLocalX < SlotSize / 2) return TileLocalY < SlotSize / 2 ? EInvTileQuadrant::TopLeft : EInvTileQuadrant::BottomLeft;
	return TileLocalY < SlotSize / 2 ? EInvTileQuadrant::TopRight : EInvTileQuadrant::BottomRight;
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

void UInvInventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (!IsValid(HoveredItem)) return;
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;

	if (CurrentSpaceQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQueryResult.UpperLeftIndex))
	{
		OnSlottedItemClicked(CurrentSpaceQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	TObjectPtr<UInvGridSlot>& GridSlot = GridSlots[ItemDropIndex];
	if (!GridSlot->GetItem().IsValid())
	{
		PlaceAtIndex(ItemDropIndex);
	}
}

void UInvInventoryGrid::PlaceAtIndex(const int32 Index)
{
	AddItemAtIndex(HoveredItem->GetItem(), Index, HoveredItem->IsStackable(), HoveredItem->GetStackCount());
	UpdateGridSlots(HoveredItem->GetItem(), Index, HoveredItem->IsStackable(), HoveredItem->GetStackCount());
	ClearHoveredItem();
}

void UInvInventoryGrid::ClearHoveredItem()
{
	if (!IsValid(HoveredItem)) return;

	HoveredItem->SetItem(nullptr);
	HoveredItem->SetPreviousIndex(INDEX_NONE);
	HoveredItem->SetStackable(false);
	HoveredItem->UpdateStackCount(0);
	HoveredItem->SetImageBrush(FSlateNoResource());

	HoveredItem->RemoveFromParent();
	HoveredItem = nullptr;

	ShowCursor();
}

bool UInvInventoryGrid::IsSameStackable(const UInvInventoryItem* ClickedItem) const
{
	const bool bSameItem = ClickedItem == HoveredItem->GetItem();
	const bool bIsStackable = ClickedItem->IsStackable();

	return bSameItem && bIsStackable && HoveredItem->GetItemTag().MatchesTagExact(ClickedItem->GetItemManifest().GetItemTag());
}

void UInvInventoryGrid::SwapWithHoveredItem(UInvInventoryItem* ClickedItem, const int32 GridIndex)
{
	if (!IsValid(HoveredItem)) return;

	UInvInventoryItem* TempItem = HoveredItem->GetItem();
	const int32 TempStackCount = HoveredItem->GetStackCount();
	const bool bTempStackable = HoveredItem->IsStackable();

	AssignHoveredItem(ClickedItem, GridIndex, HoveredItem->GetPreviousIndex());
	RemoveItemFromGrid(ClickedItem, GridIndex);
	AddItemAtIndex(TempItem, ItemDropIndex, bTempStackable, TempStackCount);
	UpdateGridSlots(TempItem, ItemDropIndex, bTempStackable, TempStackCount);
}

bool UInvInventoryGrid::ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount,
	const int32 MaxStackSize) const
{
	return RoomInClickedSlot == 0 && HoveredStackCount < MaxStackSize;
}

void UInvInventoryGrid::SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index)
{
	UInvGridSlot* GridSlot = GridSlots[Index];
	GridSlot->SetStackCount(HoveredStackCount);

	UInvSlottedItem* SlottedItem = SlottedItems.FindChecked(Index);
	SlottedItem->SetStackCount(HoveredStackCount);

	HoveredItem->UpdateStackCount(ClickedStackCount);
}

bool UInvInventoryGrid::ShouldConsumeHoveredItemStacks(const int32 HoveredStackCount,
	const int32 RoomInClickedSlot) const
{
	return RoomInClickedSlot >= HoveredStackCount;
}

void UInvInventoryGrid::ConsumeHoveredItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index)
{
	const int32 AmountToTransfer = HoveredStackCount;
	const int32 NewClickedStackCount = ClickedStackCount + AmountToTransfer;

	GridSlots[Index]->SetStackCount(NewClickedStackCount);
	SlottedItems.FindChecked(Index)->SetStackCount(NewClickedStackCount);
	ClearHoveredItem();
	ShowCursor();

	const FInvGridFragment* GridFragment = GridSlots[Index]->GetItem()->GetItemManifest().GetFragment<FInvGridFragment>();
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
	HighlightSlots(Index, Dimensions);
}

void UInvInventoryGrid::FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index)
{
	UInvGridSlot* GridSlot = GridSlots[Index];
	const int32 NewStackCount = GridSlot->GetStackCount() + FillAmount;

	GridSlot->SetStackCount(NewStackCount);

	UInvSlottedItem* ClickedSlottedItem = SlottedItems.FindChecked(Index);
	ClickedSlottedItem->SetStackCount(NewStackCount);

	HoveredItem->UpdateStackCount(Remainder);
}

UUserWidget* UInvInventoryGrid::GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(VisibleCursorClass))
	{
		VisibleCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), VisibleCursorClass);
	}
	return VisibleCursorWidget;
}

UUserWidget* UInvInventoryGrid::GetHiddenCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(HiddenCursorClass))
	{
		HiddenCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), HiddenCursorClass);
	}
	return HiddenCursorWidget;
}

void UInvInventoryGrid::ShowCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetVisibleCursorWidget());
}

void UInvInventoryGrid::HideCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetHiddenCursorWidget());
}

void UInvInventoryGrid::OnGridSlotHovered(const int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoveredItem)) return;

	UInvGridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetOccupiedTexture();
	}
}

void UInvInventoryGrid::OnGridSlotUnhovered(const int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoveredItem)) return;

	UInvGridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetUnoccupiedTexture();
	}
}

void UInvInventoryGrid::OnSlottedItemClicked(const int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UInvInventoryItem* ClickedItem = GridSlots[GridIndex]->GetItem().Get();

	if (!IsValid(HoveredItem) && IsLeftClick(MouseEvent))
	{
		PickUpItem(ClickedItem, GridIndex);
		return;
	}

	if (IsSameStackable(ClickedItem))
	{
		const int32 ClickedStackCount = GridSlots[GridIndex]->GetStackCount();
		const FInvStackableFragment* StackableFragment = ClickedItem->GetItemManifest().GetFragment<FInvStackableFragment>();
		const int32 MaxStackSize = StackableFragment->GetMaxStack();
		const int32 RoomInClickedSlot = MaxStackSize - ClickedStackCount;

		const int32 HoveredStackCount = HoveredItem->GetStackCount();
		if (ShouldSwapStackCounts(RoomInClickedSlot, HoveredStackCount, MaxStackSize))
		{
			SwapStackCounts(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}

		if (ShouldConsumeHoveredItemStacks(HoveredStackCount, RoomInClickedSlot))
		{
			ConsumeHoveredItemStacks(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}

		if (RoomInClickedSlot < HoveredStackCount)
		{
			FillInStack(RoomInClickedSlot, HoveredStackCount - RoomInClickedSlot, GridIndex);
			return;
		}
		if (RoomInClickedSlot == 0)
		{
			return;
		}
	}

	SwapWithHoveredItem(ClickedItem, GridIndex);
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
			GridSlot->OnSlotClicked.AddDynamic(this, &UInvInventoryGrid::OnGridSlotClicked);
			GridSlot->OnSlotHovered.AddDynamic(this, &UInvInventoryGrid::OnGridSlotHovered);
			GridSlot->OnSlotUnhovered.AddDynamic(this, &UInvInventoryGrid::OnGridSlotUnhovered);
		}
	}
}

FReply UInvInventoryGrid::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Only handle drag-and-drop placement if we're in dragging mode
	if (bIsDragging && IsValid(HoveredItem))
	{
		if (CurrentSpaceQueryResult.bHasSpace)
		{
			// Place the item at the current hover position (empty space)
			PlaceAtIndex(ItemDropIndex);
		}
		else if (CurrentSpaceQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentSpaceQueryResult.UpperLeftIndex))
		{
			// Swap with the item at the current hover position (occupied space)
			UInvInventoryItem* ClickedItem = CurrentSpaceQueryResult.ValidItem.Get();
			SwapWithHoveredItem(ClickedItem, CurrentSpaceQueryResult.UpperLeftIndex);
		}
		
		bIsDragging = false;
		return FReply::Handled();
	}
	
	// Reset dragging state
	bIsDragging = false;
	
	// For click-to-place, we need to let the event propagate to child widgets (GridSlots)
	return FReply::Unhandled();
}

FReply UInvInventoryGrid::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Only check for drag if mouse button is down and we have a hovered item
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && IsValid(HoveredItem) && !bIsDragging)
	{
		// If we don't have a drag start position recorded, record it now
		if (DragStartPosition.IsZero())
		{
			DragStartPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
		}
		else
		{
			FVector2D CurrentMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
			float DragDistance = FVector2D::Distance(DragStartPosition, CurrentMousePos);
			
			// If we've moved enough distance to count as a drag
			if (DragDistance > 5.0f)
			{
				bIsDragging = true;
			}
		}
	}
	else if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// Reset drag start position when mouse button is not down
		DragStartPosition = FVector2D::ZeroVector;
	}
	
	// Let the event propagate normally
	return FReply::Unhandled();
}
