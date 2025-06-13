// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InvInventoryItem.h"
#include "Items/Fragments/InvItemFragment.h" 
#include "Net/UnrealNetwork.h"

void UInvInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInvInventoryItem, ItemManifest);
	DOREPLIFETIME(UInvInventoryItem, TotalStackCount);
}

void UInvInventoryItem::SetItemManifest(const FInvItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FInvItemManifest>(Manifest);
}

bool UInvInventoryItem::IsStackable() const
{
	const FInvStackableFragment* StackableFragment = GetItemManifest().GetFragment<FInvStackableFragment>();
	return StackableFragment != nullptr;
}
