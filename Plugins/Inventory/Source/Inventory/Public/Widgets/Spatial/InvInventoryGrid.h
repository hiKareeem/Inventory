// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/InvInventoryItem.h"
#include "InvInventoryGrid.generated.h"

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

	UFUNCTION()
	void AddItem(UInvInventoryItem* Item);

private:
	void ConstructGrid();

	TWeakObjectPtr<UInvInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInvGridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
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
