// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvGridSlot.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInvGridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	int32 GetIndex() const { return SlotIndex; }
	void SetIndex(int32 Index) { SlotIndex = Index; }
	
private:
	int32 SlotIndex = -1;
};
