// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/InvInventoryUtils.h"

#include "InventoryManagment/InvInventoryComponent.h"

UInvInventoryComponent* UInvInventoryUtils::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	
	return PlayerController->FindComponentByClass<UInvInventoryComponent>();
}
