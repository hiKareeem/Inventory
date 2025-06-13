// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InvInventoryItem.h"

#include "Net/UnrealNetwork.h"

void UInvInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInvInventoryItem, ItemManifest);
}

void UInvInventoryItem::SetItemManifest(const FInvItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FInvItemManifest>(Manifest);
}
