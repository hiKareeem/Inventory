// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "InvSlottedItem.generated.h"

class UTextBlock;
class UImage;
class UInvInventoryItem;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvSlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:
	bool IsStackable() const { return bStackable; }
	void SetStackable(const bool bIsStackable) { bStackable = bIsStackable; }
	
	UImage* GetImage() { return Image_Item; }
	void SetImage(const FSlateBrush& Brush) const { Image_Item->SetBrush(Brush); }
	
	int32 GetGridIndex() const { return GridIndex; }
	void SetGridIndex(const int32 Index) { GridIndex = Index; }
	
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetGridDimensions(const FIntPoint Dimensions) { GridDimensions = Dimensions; }
	
	UInvInventoryItem* GetItem() const { return Item.Get(); }
	void SetItem(UInvInventoryItem* NewItem);

	void SetStackCount(const int32 Count);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 GridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UInvInventoryItem> Item;
	bool bStackable = false;
};
