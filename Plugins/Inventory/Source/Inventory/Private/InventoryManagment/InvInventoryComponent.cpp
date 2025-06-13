// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/InvInventoryComponent.h"
#include "Items/InvItemComponent.h"
#include "InvGridTypes.h"
#include "Net/UnrealNetwork.h"
#include "Items/InvInventoryItem.h"
#include "Items/Fragments/InvItemFragment.h"
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

	UInvInventoryItem* FoundItem = InventoryList.FindFirstItemByTag(ItemComponent->GetItemManifest().GetItemTag());
	Result.Item = FoundItem;

	if (Result.TotalRoomToFill == 0 )
	{
		OnInventoryFull.Broadcast();
		return;
	}
	
	if (Result.Item.IsValid() && Result.bStackable)
	{
		OnStackChanged.Broadcast(Result);
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
	Item->SetTotalStackCount(StackCount);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(Item);
	}

	ItemComponent->PickedUp();
}

void UInvInventoryComponent::Server_AddStackableItem_Implementation(UInvItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
	const FGameplayTag& ItemTag = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemTag() : FGameplayTag::EmptyTag;
	UInvInventoryItem* Item = InventoryList.FindFirstItemByTag(ItemTag);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);
	
	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FInvStackableFragment* StackableFragment =  ItemComponent->GetItemManifest().GetFragmentMutable<FInvStackableFragment>())
	{
		StackableFragment->SetCurrentStack(Remainder);
	}
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
