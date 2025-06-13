// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InvHoverItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/InvInventoryItem.h"

void UInvHoverItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Item->SetBrush(Brush);
}

void UInvHoverItem::UpdateStackCount(const int32 Count) const
{
	if (Count > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(Count));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag UInvHoverItem::GetItemTag() const
{
	if (Item.IsValid()) return Item->GetItemManifest().GetItemTag();
	return FGameplayTag::EmptyTag; 
}

void UInvHoverItem::SetStackable(const bool bIsStackable)
{
	bStackable = bIsStackable;
	if (!bStackable)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UInvInventoryItem* UInvHoverItem::GetItem() const
{
	return Item.Get();
}

void UInvHoverItem::SetItem(UInvInventoryItem* NewItem)
{
	Item = NewItem;
}
