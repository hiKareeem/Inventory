// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/InvSlottedItem.h"
#include "Items/InvInventoryItem.h"

void UInvSlottedItem::SetItem(UInvInventoryItem* NewItem)
{
	Item = NewItem;
}
