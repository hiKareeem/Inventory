// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Composite/InvComposite.h"
#include "InvItemDescription.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvItemDescription : public UInvComposite
{
	GENERATED_BODY()

public:
	FVector2D GetBoxSize() const { return SizeBox_Base->GetDesiredSize(); }

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Base;
};
