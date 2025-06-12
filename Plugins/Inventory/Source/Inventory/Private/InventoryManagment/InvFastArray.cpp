#include "InventoryManagment/InvFastArray.h"

#include "InventoryManagment/InvInventoryComponent.h"
#include "Items/InvInventoryItem.h"

TArray<TObjectPtr<UInvInventoryItem>> FInvInventoryFastArray::GetAllItems() const
{
	TArray<TObjectPtr<UInvInventoryItem>> Results;
	Results.Reserve(Entries.Num());
	for (const auto& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}
	return Results;
}

void FInvInventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UInvInventoryComponent* InventoryComponent = Cast<UInvInventoryComponent>(OwningComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAdded.Broadcast(Entries[Index].Item);
	}
}

void FInvInventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UInvInventoryComponent* InventoryComponent = Cast<UInvInventoryComponent>(OwningComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : RemovedIndices)
	{
		InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].Item);
	}
}

UInvInventoryItem* FInvInventoryFastArray::AddEntry(UInvItemComponent* ItemComponent)
{
	return nullptr;
}

UInvInventoryItem* FInvInventoryFastArray::AddEntry(UInvInventoryItem* Item)
{
	check(OwningComponent);
	AActor* OwningActor = OwningComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FInvInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;
	
	MarkItemDirty(NewEntry);
	
	return Item;
}

void FInvInventoryFastArray::RemoveEntry(UInvInventoryItem* Item)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FInvInventoryEntry& Entry = *It;
		if (Entry.Item == Item)
		{
			It.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
