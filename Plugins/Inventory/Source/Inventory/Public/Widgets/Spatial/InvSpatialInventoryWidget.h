// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InvInventoryWidgetBase.h"
#include "InvSpatialInventoryWidget.generated.h"

class UCanvasPanel;
class UInvItemDescription;
class UInvInventoryGrid;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvSpatialInventoryWidget : public UInvInventoryWidgetBase
{
	GENERATED_BODY()

public:
	virtual FInvSlotAvailabilityResult HasRoomForItem(UInvItemComponent* ItemComponent) const override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnItemHovered(UInvInventoryItem* Item) override;
	virtual void OnItemUnhovered() override;
	virtual bool HasHoverItem() const override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInvInventoryGrid> InventoryGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInvItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UInvItemDescription> ItemDescriptionWidget;

	UInvItemDescription* GetItemDescription();

	FTimerHandle ItemDescriptionTimer;
	void SetItemDescriptionSizeAndPosition(UInvItemDescription* ItemDescription, const UCanvasPanel* Canvas) const;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float ItemDescriptionDelay = 0.5f;
};
