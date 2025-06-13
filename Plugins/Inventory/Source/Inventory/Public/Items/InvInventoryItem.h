// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/InvItemManifest.h"
#include "InvInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInvInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

	const FInvItemManifest& GetItemManifest() const { return ItemManifest.Get<FInvItemManifest>(); }
	FInvItemManifest& GetMutableItemManifest() { return ItemManifest.GetMutable<FInvItemManifest>(); }
	void SetItemManifest(const FInvItemManifest& Manifest);
	
private:

	UPROPERTY(VisibleAnywhere, Replicated, meta = (BaseStruct = "/Script/Inventory.InvItemManifest"), Category = "Inventory")
	FInstancedStruct ItemManifest;
};

template<typename FragmentType>
const FragmentType* GetFragment(const UInvInventoryItem* Item, const FGameplayTag& FragmentTag)
{
	if (!IsValid(Item)) return nullptr;

	const FInvItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentByTag<FragmentType>(FragmentTag);
}