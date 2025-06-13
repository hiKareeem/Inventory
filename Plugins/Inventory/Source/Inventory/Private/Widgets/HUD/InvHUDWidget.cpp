// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/InvHUDWidget.h"

#include "InventoryManagment/InvInventoryComponent.h"
#include "InventoryManagment/InvInventoryUtils.h"
#include "Widgets/HUD/InvInfoMessageWidget.h"

void UInvHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UInvInventoryComponent* InventoryComponent = UInvInventoryUtils::GetInventoryComponent(GetOwningPlayer());
	if (!IsValid(InventoryComponent)) return;

	InventoryComponent->OnInventoryFull.AddDynamic(this, &UInvHUDWidget::OnInventoryFull);
}

void UInvHUDWidget::OnInventoryFull()
{
	if (!IsValid(InfoMessageWidget)) return;
	InfoMessageWidget->SetMessage(FText::FromString("You can't hold any more items!"));
}
