#include "Items/InvItemManifest.h" 
#include "Items/InvInventoryItem.h"

UInvInventoryItem* FInvItemManifest::Manifest(UObject* Outer) const
{
	UInvInventoryItem* Item = NewObject<UInvInventoryItem>(Outer, UInvInventoryItem::StaticClass());
	Item->SetItemManifest(*this);
	return Item;
}
