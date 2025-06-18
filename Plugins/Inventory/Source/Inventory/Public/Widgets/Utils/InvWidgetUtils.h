// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InvWidgetUtils.generated.h"

class UWidget;
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

	UFUNCTION(BlueprintPure, Category = "Inventory|Utils", meta = (WorldContext = "WorldContextObject"))
	static FVector2D GetWidgetPosition(UWidget* Widget);

	UFUNCTION(BlueprintPure, Category = "Inventory|Utils", meta = (WorldContext = "WorldContextObject"))
	static bool IsWithinBounds(const FVector2D& BoundaryPosition, const FVector2D& WidgetSize, const FVector2D& MousePosition);

	UFUNCTION(BlueprintPure, Category = "Inventory|Utils", meta = (WorldContext = "WorldContextObject"))
	static FVector2D GetWidgetSize(const UWidget* Widget);

	UFUNCTION(BlueprintPure, Category = "Inventory|Utils", meta = (WorldContext = "WorldContextObject"))
	static FVector2D GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, const FVector2D& MousePosition);
};
