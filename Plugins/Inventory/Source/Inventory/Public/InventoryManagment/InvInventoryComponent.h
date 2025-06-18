// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvFastArray.h"
#include "Components/ActorComponent.h"
#include "InvInventoryComponent.generated.h"

struct FInvSlotAvailabilityResult;
class UInvItemComponent;
class UInvInventoryItem;
class UInvInventoryWidgetBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, UInvInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryFull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChanged, const FInvSlotAvailabilityResult&, Result);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInvInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInvInventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UInvItemComponent* ItemComponent);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInvItemComponent* ItemComponent, int32 StackCount);
	
	UFUNCTION(Server, Reliable)
	void Server_AddStackableItem(UInvItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	void AddRepSubobject(UObject* Subobject);

	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;
	FInventoryFull OnInventoryFull;
	FStackChanged OnStackChanged;

	UInvInventoryWidgetBase* GetInventoryWidget() const { return InventoryWidget; }

protected:
	virtual void BeginPlay() override;

private:
	void ConstructInventory();

	UPROPERTY(Replicated)
	FInvInventoryFastArray InventoryList;
	
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|UI")
	TSubclassOf<UInvInventoryWidgetBase> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UInvInventoryWidgetBase> InventoryWidget;
};
