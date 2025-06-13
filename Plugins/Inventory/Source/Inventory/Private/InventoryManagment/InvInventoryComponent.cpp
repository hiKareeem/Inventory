// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/InvInventoryComponent.h"

#include "Widgets/Inventory/InvInventoryWidgetBase.h"


UInvInventoryComponent::UInvInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInvInventoryComponent::TryAddItem(UInvItemComponent* ItemComponent)
{
	OnInventoryFull.Broadcast();
}

void UInvInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
}

void UInvInventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	if (!OwningPlayerController->IsLocalController()) return;
	
	InventoryWidget = CreateWidget<UInvInventoryWidgetBase>(OwningPlayerController.Get(), InventoryWidgetClass);
	InventoryWidget->AddToViewport();
}
