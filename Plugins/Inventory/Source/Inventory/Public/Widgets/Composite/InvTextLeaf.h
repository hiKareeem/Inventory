// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvLeaf.h"
#include "InvTextLeaf.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvTextLeaf : public UInvLeaf
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	void SetText(const FText& NewText);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LeafTextBlock;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 FontSize = 12;
};
