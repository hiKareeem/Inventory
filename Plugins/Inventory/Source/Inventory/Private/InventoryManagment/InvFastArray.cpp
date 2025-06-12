#include "InventoryManagment/InvFastArray.h"

TArray<TObjectPtr<UInvInventoryItem>> FInvInventoryFastArray::GetAllItems() const
{
}

void FInvInventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FInvInventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

UInvInventoryItem* FInvInventoryFastArray::AddEntry(UInvItemComponent* ItemComponent)
{
}

UInvInventoryItem* FInvInventoryFastArray::AddEntry(UInvInventoryItem* Item)
{
}

void FInvInventoryFastArray::RemoveEntry(UInvInventoryItem* Item)
{
}
