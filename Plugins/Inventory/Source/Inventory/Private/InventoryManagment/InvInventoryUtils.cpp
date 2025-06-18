// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/InvInventoryUtils.h"

#include "InventoryManagment/InvInventoryComponent.h"
#include "Items/InvItemComponent.h"
#include "Widgets/Inventory/InvInventoryWidgetBase.h"

UInvInventoryComponent* UInvInventoryUtils::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	
	return PlayerController->FindComponentByClass<UInvInventoryComponent>();
}

EInvItemCategory UInvInventoryUtils::GetItemCategoryFromItemComponent(UInvItemComponent* ItemComponent)
{
	if (!IsValid(ItemComponent)) return EInvItemCategory::None;
	return ItemComponent->GetItemManifest().GetCategory(); 
}

void UInvInventoryUtils::ItemHovered(APlayerController* PlayerController, UInvInventoryItem* Item)
{
	UInvInventoryComponent* InventoryComponent = GetInventoryComponent(PlayerController);
	if (!IsValid(InventoryComponent)) return;

	UInvInventoryWidgetBase* InventoryWidget = InventoryComponent->GetInventoryWidget();
	if (!IsValid(InventoryWidget)) return;
	if (InventoryWidget->HasHoverItem()) return;

	InventoryWidget->OnItemHovered(Item);
}

void UInvInventoryUtils::ItemUnhovered(APlayerController* PlayerController)
{
	UInvInventoryComponent* InventoryComponent = GetInventoryComponent(PlayerController);
	if (!IsValid(InventoryComponent)) return;

	UInvInventoryWidgetBase* InventoryWidget = InventoryComponent->GetInventoryWidget();
	if (!IsValid(InventoryWidget)) return;

	InventoryWidget->OnItemUnhovered();
}
