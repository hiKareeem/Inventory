// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvGridTypes.h"
#include "Blueprint/UserWidget.h"
#include "InvInventoryWidgetBase.generated.h"

class UInvItemComponent;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInvInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FInvSlotAvailabilityResult HasRoomForItem(UInvItemComponent* ItemComponent) const { return FInvSlotAvailabilityResult(); }
};
