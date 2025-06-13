// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
};
