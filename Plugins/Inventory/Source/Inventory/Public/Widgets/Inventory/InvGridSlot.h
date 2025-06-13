// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Items/InvInventoryItem.h"
#include "InvGridSlot.generated.h"

UENUM(BlueprintType)
enum class EInvGridSlotState : uint8
{
	Unoccupied = 0 UMETA(DisplayName = "Unoccupied"),
	Occupied = 1 UMETA(DisplayName = "Occupied"),
	Selected = 2 UMETA(DisplayName = "Selected"),
	Invalid = 3 UMETA(DisplayName = "Invalid")
};

/**
 * 
 */
UCLASS()
class INVENTORY_API UInvGridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	int32 GetIndex() const { return SlotIndex; }
	void SetIndex(int32 Index) { SlotIndex = Index; }
	EInvGridSlotState GetState() const { return SlotState; }
	TWeakObjectPtr<UInvInventoryItem> GetItem() const { return Item; }
	void SetItem(UInvInventoryItem* InItem) { Item = InItem; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }
	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }
	bool IsAvailable() const { return bAvailable; }
	void SetAvailable(bool bInAvailable) { bAvailable = bInAvailable; }
	
	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetInvalidTexture();
	
private:
	int32 SlotIndex = INDEX_NONE;
	int32 StackCount = 0;
	int32 UpperLeftIndex = INDEX_NONE;
	bool bAvailable = true;
	TWeakObjectPtr<UInvInventoryItem> Item;
	EInvGridSlotState SlotState = EInvGridSlotState::Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_Invalid;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Slot;
};
