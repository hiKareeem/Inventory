// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/InvInventoryItem.h"
#include "Items/Fragments/InvItemFragment.h"
#include "InvInventoryGrid.generated.h"

class UInvSlottedItem;
class UInvItemComponent;
class UInvInventoryItem;
class UInvInventoryComponent;
class UCanvasPanel;
class UInvGridSlot;
enum class EInvItemCategory : uint8;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	EInvItemCategory GetItemCategory() const { return ItemCategory; }
	FInvSlotAvailabilityResult HasRoomForItem(const UInvItemComponent* ItemComponent) const;

	UFUNCTION()
	void AddItem(UInvInventoryItem* Item);

private:
	void ConstructGrid();
	FInvSlotAvailabilityResult HasRoomForItem(const UInvInventoryItem* Item) const;
	FInvSlotAvailabilityResult HasRoomForItem(const FInvItemManifest& Manifest) const;
	void AddItemToIndices(const FInvSlotAvailabilityResult& Result, UInvInventoryItem* Item);
	UInvSlottedItem* CreateSlottedItem(UInvInventoryItem* Item, int32 Index, const FInvGridFragment* GridFragment,
	                       const FInvImageFragment* ImageFragment) const;
	FVector2D GetDrawSize(const FInvGridFragment* GridFragment) const;
	void SetSlottedItemImage(const UInvSlottedItem* SlottedItem, const FInvGridFragment* GridFragment, const FInvImageFragment* ImageFragment) const;
	void AddItemAtIndex(UInvInventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	void AddSlottedItemToCanvas(const int32 Index, const FInvGridFragment* GridFragment, UInvSlottedItem* SlottedItem) const;

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
	EInvItemCategory ItemCategory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows = 1;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns = 1;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float SlotSize = 100.0f;
	
	UPROPERTY()
	TArray<TObjectPtr<UInvGridSlot>> GridSlots;

	bool MatchesCategory(const UInvInventoryItem* Item) const { return Item->GetItemManifest().GetCategory() == ItemCategory; }
};
