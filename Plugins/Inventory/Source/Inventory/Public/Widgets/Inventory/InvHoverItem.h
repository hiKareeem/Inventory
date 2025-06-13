// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "InvHoverItem.generated.h"

class UTextBlock;
class UInvInventoryItem;
class UImage;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvHoverItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetImageBrush(const FSlateBrush& Brush) const;
	void UpdateStackCount(int32 Count);

	FGameplayTag GetItemTag() const;
	int32 GetStackCount() const { return StackCount; }
	
	bool IsStackable() const { return bStackable; }
	void SetStackable(const bool bIsStackable);
	
	int32 GetPreviousIndex() const { return PreviousIndex; }
	void SetPreviousIndex(const int32 Index) { PreviousIndex = Index; }
	
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }
	
	UInvInventoryItem* GetItem() const;
	void SetItem(UInvInventoryItem* NewItem);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 PreviousIndex = INDEX_NONE;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UInvInventoryItem> Item;
	bool bStackable = false;
	int32 StackCount = 0;
};
