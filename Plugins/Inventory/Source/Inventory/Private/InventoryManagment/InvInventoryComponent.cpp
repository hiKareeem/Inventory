// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/InvInventoryComponent.h"
#include "Items/InvItemComponent.h"
#include "InvGridTypes.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/InvInventoryWidgetBase.h"


UInvInventoryComponent::UInvInventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UInvInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInvInventoryComponent, InventoryList);
}

void UInvInventoryComponent::TryAddItem(UInvItemComponent* ItemComponent)
{
	FInvSlotAvailabilityResult Result = InventoryWidget->HasRoomForItem(ItemComponent);

	if (Result.TotalRoomToFill == 0 )
	{
		OnInventoryFull.Broadcast();
		return;
	}
	
	if (Result.Item.IsValid() && Result.bStackable)
	{
		Server_AddStackableItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0)
	{
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
	}
}

void UInvInventoryComponent::AddRepSubobject(UObject* Subobject)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(Subobject))
	{
		AddReplicatedSubObject(Subobject);
	}
}

void UInvInventoryComponent::Server_AddNewItem_Implementation(UInvItemComponent* ItemComponent, int32 StackCount)
{
	UInvInventoryItem* Item = InventoryList.AddEntry(ItemComponent);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(Item);
	}
}

void UInvInventoryComponent::Server_AddStackableItem_Implementation(UInvItemComponent* ItemComponent, int32 StackCount,
	int32 Remainder)
{
	
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
