// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/InvImageLeaf.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"

void UInvImageLeaf::SetImage(UTexture2D* Texture) const
{
	Image->SetBrushFromTexture(Texture);
}

void UInvImageLeaf::SetBoxSize(const FVector2D& Size) const
{
	SizeBox->SetWidthOverride(Size.X);
	SizeBox->SetHeightOverride(Size.Y);
}

void UInvImageLeaf::SetImageSize(const FVector2D& Size) const
{
	Image->SetDesiredSizeOverride(Size);
}

FVector2D UInvImageLeaf::GetImageSize() const
{
	return Image->GetDesiredSize(); 
}
