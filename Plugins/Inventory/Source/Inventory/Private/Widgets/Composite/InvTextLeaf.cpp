// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/InvTextLeaf.h"

#include "Components/TextBlock.h"

void UInvTextLeaf::NativePreConstruct()
{
	Super::NativePreConstruct();

	FSlateFontInfo SlateFontInfo = LeafTextBlock->GetFont();
	SlateFontInfo.Size = FontSize;
	
	LeafTextBlock->SetFont(SlateFontInfo);
}

void UInvTextLeaf::SetText(const FText& NewText)
{
	LeafTextBlock->SetText(NewText);
}
