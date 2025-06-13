// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InvInventoryComponent.generated.h"

class UInvItemComponent;
class UInvInventoryItem;
class UInvInventoryWidgetBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, UInvInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryFull);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInvInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInvInventoryComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UInvItemComponent* ItemComponent);

	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;
	FInventoryFull OnInventoryFull;

protected:
	virtual void BeginPlay() override;

private:
	void ConstructInventory();
	
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|UI")
	TSubclassOf<UInvInventoryWidgetBase> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UInvInventoryWidgetBase> InventoryWidget;
};
