// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Spatial/InvSpatialInventoryWidget.h"

#include "InventoryManagment/InvInventoryUtils.h"
#include "Widgets/Spatial/InvInventoryGrid.h"

FInvSlotAvailabilityResult UInvSpatialInventoryWidget::HasRoomForItem(UInvItemComponent* ItemComponent) const
{
	switch (UInvInventoryUtils::GetItemCategoryFromItemComponent(ItemComponent))
	{
		case EInvItemCategory::Equippable:
			return InventoryGrid->HasRoomForItem(ItemComponent);
		default:
			return FInvSlotAvailabilityResult();
	};
}
