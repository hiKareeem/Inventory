// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/InvInventoryItem.h"
#include "Items/Fragments/InvItemFragment.h"
#include "InvInventoryGrid.generated.h"

enum class EInvGridSlotState : uint8;
enum class EInvItemCategory : uint8;
class UInvHoverItem;
class UInvSlottedItem;
class UInvItemComponent;
class UInvInventoryItem;
class UInvInventoryComponent;
class UCanvasPanel;
class UInvGridSlot;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	EInvItemCategory GetItemCategory() const { return ItemCategory; }
	FInvSlotAvailabilityResult HasRoomForItem(const UInvItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(UInvInventoryItem* Item);

private:
	void ConstructGrid();
	FInvSlotAvailabilityResult HasRoomForItem(const UInvInventoryItem* Item);
	FInvSlotAvailabilityResult HasRoomForItem(const FInvItemManifest& Manifest);
	bool HasRoomAtIndex(const UInvGridSlot* GridSlot,
						const FIntPoint& Dimensions,
						const TSet<int32>& CheckedIndices,
						TSet<int32>& OutTentativelyClaimed,
						const FGameplayTag& ItemType,
						const int32 MaxStackSize);
	void AddItemToIndices(const FInvSlotAvailabilityResult& Result, UInvInventoryItem* Item);
	UInvSlottedItem* CreateSlottedItem(UInvInventoryItem* Item,
										const int32 Index,
										const bool bStackable,
										int32 StackAmount,
										const FInvGridFragment* GridFragment,
									    const FInvImageFragment* ImageFragment) const;
	FVector2D GetDrawSize(const FInvGridFragment* GridFragment) const;
	void SetSlottedItemImage(const UInvSlottedItem* SlottedItem, const FInvGridFragment* GridFragment, const FInvImageFragment* ImageFragment) const;
	void AddItemAtIndex(UInvInventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	void AddSlottedItemToCanvas(const int32 Index, const FInvGridFragment* GridFragment, UInvSlottedItem* SlottedItem) const;
	void UpdateGridSlots(UInvInventoryItem* Item, const int32 Index, bool bStackable, int32 StackAmount);
	FIntPoint GetDimensions(const FInvItemManifest& Manifest) const;
	bool CheckSlotConstraints(const UInvGridSlot* GridSlot, const UInvGridSlot* SubGridSlot,
	                          const TSet<int32>& CheckedIndices, TSet<int32>&
	                          TentativelyClaimedIndices, const FGameplayTag& ItemTag, const int32 MaxStackSize) const;
	bool HasValidItem(const UInvGridSlot* GridSlot) const;
	bool IsUpperLeftSlot(const UInvGridSlot* GridSlot, const UInvGridSlot* SubGridSlot) const;
	bool DoesItemTypeMatch(const UInvInventoryItem* Item, const FGameplayTag& ItemTag) const;
	bool IsInGridBounds(const int32 StartingIndex, const FIntPoint& Dimensions) const;
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInvGridSlot* GridSlot) const;
	int32 GetStackAmount(const UInvGridSlot* GridSlot) const;
	bool MatchesCategory(const UInvInventoryItem* Item) const { return Item->GetItemManifest().GetCategory() == ItemCategory; }
	
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	void PickUpItem(UInvInventoryItem* ClickedItem, const int32 GridIndex);
	void AssignHoveredItem(UInvInventoryItem* ClickedItem);
	void AssignHoveredItem(UInvInventoryItem* ClickedItem, const int32 GridIndex, const int32 PreviousGridIndex);
	void RemoveItemFromGrid(UInvInventoryItem* ClickedItem, const int32 GridIndex);
	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	EInvTileQuadrant DetermineHoveredQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	void OnTileParametersChanged(const FInvTileParameters& InTileParameters);
	FIntPoint CalculateStartingCoordinates(const FIntPoint& Coordinates, const FIntPoint& Dimensions, const EInvTileQuadrant Quadrant) const;
	FInvSpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);
	bool CursorExitedCanvas(const FVector2D& BoundaryPosition, const FVector2D& BoundarySize, const FVector2D& Location);
	void HighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void UnhighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EInvGridSlotState GridSlotState);
	void PlaceAtIndex(const int32 Index);
	void ClearHoveredItem();
	bool IsSameStackable(const UInvInventoryItem* ClickedItem) const;
	void SwapWithHoveredItem(UInvInventoryItem* ClickedItem, const int32 GridIndex);
	void HandleItemPlacementOnOccupiedSlot(UInvInventoryItem* ClickedItem, const int32 GridIndex);

	bool ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount, const int32 MaxStackSize) const;
	void SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);
	bool ShouldConsumeHoveredItemStacks(const int32 HoveredStackCount, const int32 RoomInClickedSlot) const;
	void ConsumeHoveredItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);
	void FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index);
	
	UUserWidget* GetVisibleCursorWidget();
	UUserWidget* GetHiddenCursorWidget();
	void ShowCursor();
	void HideCursor();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UUserWidget> VisibleCursorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UUserWidget> HiddenCursorClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> VisibleCursorWidget;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> HiddenCursorWidget;

	UFUNCTION()
	void AddStacks(const FInvSlotAvailabilityResult& Result);

	UFUNCTION()
	void OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent);
	
	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);
	
	UFUNCTION()
	void OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	TWeakObjectPtr<UInvInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInvGridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInvSlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UInvSlottedItem>> SlottedItems;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInvHoverItem> HoveredItemClass;

	UPROPERTY()
	TObjectPtr<UInvHoverItem> HoveredItem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	EInvItemCategory ItemCategory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows = 1;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns = 1;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float SlotSize = 100.0f;
	
	UPROPERTY()
	TArray<TObjectPtr<UInvGridSlot>> GridSlots;

	FInvTileParameters TileParameters;
	FInvTileParameters LastTileParameters;
	int32 ItemDropIndex = INDEX_NONE;
	FInvSpaceQueryResult CurrentSpaceQueryResult;
	bool bMouseWithinCanvas = false;
	bool bLastMouseWithinCanvas = false;
	int32 LastHighlightedIndex = INDEX_NONE;
	FIntPoint LastHighlightedDimensions;

	// Track whether we're in drag mode
	bool bIsDragging = false;
	FVector2D DragStartPosition;
};
