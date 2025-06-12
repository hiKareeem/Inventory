// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvInventoryGrid.generated.h"

enum class EInvItemCategory : uint8;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	EInvItemCategory GetItemCategory() const { return ItemCategory; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInvItemCategory ItemCategory;
};
