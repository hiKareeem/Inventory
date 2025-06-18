// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Spatial/InvSpatialInventoryWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagment/InvInventoryUtils.h"
#include "Widgets/InvItemDescription.h"
#include "Widgets/Spatial/InvInventoryGrid.h"

FInvSlotAvailabilityResult UInvSpatialInventoryWidget::HasRoomForItem(UInvItemComponent* ItemComponent) const
{
	switch (UInvInventoryUtils::GetItemCategoryFromItemComponent(ItemComponent))
	{
		case EInvItemCategory::Equippable:
			return InventoryGrid->HasRoomForItem(ItemComponent);
		default:
			return FInvSlotAvailabilityResult();
	};
}

void UInvSpatialInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ItemDescriptionWidget) return;
	SetItemDescriptionSizeAndPosition(ItemDescriptionWidget, CanvasPanel);	
}

void UInvSpatialInventoryWidget::OnItemHovered(UInvInventoryItem* Item)
{
	const FInvItemManifest ItemManifest = Item->GetItemManifest();
	UInvItemDescription* ItemDescription = GetItemDescription();
	ItemDescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ItemDescriptionTimer);

	FTimerDelegate ItemDescriptionDelegate;
	ItemDescriptionDelegate.BindLambda([this, &ItemManifest, ItemDescription]()
	{
		ItemManifest.AssimilateInventoryFragments(ItemDescription);
		ItemDescriptionWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	});

	GetOwningPlayer()->GetWorldTimerManager().SetTimer(ItemDescriptionTimer, ItemDescriptionDelegate, ItemDescriptionDelay, false);
}

void UInvSpatialInventoryWidget::OnItemUnhovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ItemDescriptionTimer);
}

bool UInvSpatialInventoryWidget::HasHoverItem() const
{
	if (InventoryGrid->HasHoverItem()) return true;

	return false;
}

UInvItemDescription* UInvSpatialInventoryWidget::GetItemDescription()
{
	if (!IsValid(ItemDescriptionWidget))
	{
		ItemDescriptionWidget = CreateWidget<UInvItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescriptionWidget);
	}
	return ItemDescriptionWidget;
}

void UInvSpatialInventoryWidget::SetItemDescriptionSizeAndPosition(UInvItemDescription* ItemDescription,
	const UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemDescription);

	const FVector2D ItemDescriptionSize = ItemDescription->GetBoxSize();
	ItemDescriptionSlot->SetSize(ItemDescriptionSize);

	const FVector2D ClampedPosition = UInvWidgetUtils::GetClampedWidgetPosition(
		UInvWidgetUtils::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()) - FVector2D(0, ItemDescriptionSize.Y)
	);
		
	ItemDescriptionSlot->SetPosition(ClampedPosition);
}
