// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InvInventoryWidgetBase.h"
#include "InvSpatialInventoryWidget.generated.h"

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
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInvInventoryGrid> InventoryGrid;
};
