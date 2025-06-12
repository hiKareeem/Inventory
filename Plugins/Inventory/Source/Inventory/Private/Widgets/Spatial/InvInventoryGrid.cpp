// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Spatial/InvInventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Inventory/InvGridSlot.h" 
#include "Widgets/Utils/InvWidgetUtils.h"

void UInvInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();
}

void UInvInventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int32 Row = 0; Row < Rows; Row++)
	{
		for (int32 Column = 0; Column < Columns; Column++)
		{
			UInvGridSlot* GridSlot = CreateWidget<UInvGridSlot>(this, GridSlotClass);
			CanvasPanel->AddChildToCanvas(GridSlot);

			FIntPoint SlotPosition(Column, Row);
			GridSlot->SetIndex(UInvWidgetUtils::GetIndexFromSlotPosition(SlotPosition, Columns));

			UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			CanvasSlot->SetSize(FVector2D(SlotSize));
			CanvasSlot->SetPosition(SlotPosition * SlotSize);
			
			GridSlots.Add(GridSlot);
		}
	}
}
