// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Utils/InvWidgetUtils.h"
#include "InvInventoryUtils.generated.h"

class UInvItemComponent;
enum class EInvItemCategory : uint8;
class UInvInventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvInventoryUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static UInvInventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static EInvItemCategory GetItemCategoryFromItemComponent(UInvItemComponent* ItemComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static void ItemHovered(APlayerController* PlayerController, UInvInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (WorldContext = "WorldContextObject"))
	static void ItemUnhovered(APlayerController* PlayerController);
	
	template <typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Func);
};

template <typename T, typename FuncT>
void UInvInventoryUtils::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Func)
{
	for (int32 Row = 0; Row < Range2D.Y; Row++)
	{
		for (int32 Column = 0; Column < Range2D.X; Column++)
		{
			const FIntPoint Coordinates = UInvWidgetUtils::GetSlotPositionFromIndex(Index, GridColumns) + FIntPoint(Column, Row);
			const int32 TileIndex = UInvWidgetUtils::GetIndexFromSlotPosition(Coordinates, GridColumns);

			if (Array.IsValidIndex(TileIndex))
			{
				Func(Array[TileIndex]);
			}
		}
	}
}
