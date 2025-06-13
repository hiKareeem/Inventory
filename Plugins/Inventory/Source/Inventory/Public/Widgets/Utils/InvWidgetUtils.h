// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InvWidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInvWidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static int32 GetIndexFromSlotPosition(FIntPoint SlotPosition, const int32 Columns);
	static FIntPoint GetSlotPositionFromIndex(int32 Index, const int32 Columns);
};
