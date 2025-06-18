#include "Items/Fragments/InvItemFragment.h"

#include "Widgets/Composite/InvCompositeBase.h"
#include "Widgets/Composite/InvImageLeaf.h"

void FInvInventoryItemFragment::Assimilate(UInvCompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInvInventoryItemFragment::MatchesWidgetTag(const UInvCompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInvImageFragment::Assimilate(UInvCompositeBase* Composite) const
{
	FInvInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInvImageLeaf* ImageLeaf = Cast<UInvImageLeaf>(Composite);
	if (!IsValid(ImageLeaf)) return;

	ImageLeaf->SetImage(Image);
	ImageLeaf->SetBoxSize(ImageSize);
	ImageLeaf->SetImageSize(ImageSize);
}
