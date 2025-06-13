// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/InvItemManifest.h"
#include "InvInventoryItem.generated.h"

struct FInvStackableFragment;
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
	int32 GetTotalStackCount() const { return TotalStackCount; }
	void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }
	bool IsStackable() const;
	
private:
	UPROPERTY(VisibleAnywhere, Replicated, meta = (BaseStruct = "/Script/Inventory.InvItemManifest"), Category = "Inventory")
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount = 0;
};

template<typename FragmentType>
const FragmentType* GetFragment(const UInvInventoryItem* Item, const FGameplayTag& FragmentTag)
{
	if (!IsValid(Item)) return nullptr;

	const FInvItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentByTag<FragmentType>(FragmentTag);
}