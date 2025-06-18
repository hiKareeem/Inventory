#include "Items/InvItemManifest.h" 
#include "Items/InvInventoryItem.h"
#include "Items/Fragments/InvItemFragment.h"
#include "Widgets/Composite/InvCompositeBase.h"

UInvInventoryItem* FInvItemManifest::Manifest(UObject* Outer) const
{
	UInvInventoryItem* Item = NewObject<UInvInventoryItem>(Outer, UInvInventoryItem::StaticClass());
	Item->SetItemManifest(*this);
	return Item;
}

void FInvItemManifest::AssimilateInventoryFragments(UInvCompositeBase* Composite) const
{
	const auto FoundFragments = GetAllFragmentsOfType<FInvInventoryItemFragment>();
	for (const FInvInventoryItemFragment* Fragment : FoundFragments)
	{
		Composite->ApplyFunction([Fragment](UInvCompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
}
