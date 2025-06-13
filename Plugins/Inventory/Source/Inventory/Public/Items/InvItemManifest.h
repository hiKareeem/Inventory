#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InvGridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "InvItemManifest.generated.h"

struct FInvItemFragment;

USTRUCT(BlueprintType)
struct INVENTORY_API FInvItemManifest
{
	GENERATED_BODY()

public:
	UInvInventoryItem* Manifest(UObject* Outer) const;
	EInvItemCategory GetCategory() const { return Category; }
	const FGameplayTag& GetItemTag() const { return ItemTag; }

	template<typename T> requires std::derived_from<T, FInvItemFragment>
	const T* GetFragmentByTag(const FGameplayTag& FragmentTag) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInvItemFragment>> Fragments;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	EInvItemCategory Category = EInvItemCategory::None;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (Categories = "GameItems"))
	FGameplayTag ItemTag;
};

template<typename T> requires std::derived_from<T, FInvItemFragment>
const T* FInvItemManifest::GetFragmentByTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FInvItemFragment>& Fragment : Fragments)
	{
		if (const T* FoundFragment = Fragment.GetPtr<T>())
		{
			if (!FoundFragment->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
			return FoundFragment; 
		}
	}	
	return nullptr;
}