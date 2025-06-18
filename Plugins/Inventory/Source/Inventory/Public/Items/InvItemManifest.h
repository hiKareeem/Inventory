#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InvGridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "InvItemManifest.generated.h"

class UInvCompositeBase;
struct FInvItemFragment;

USTRUCT(BlueprintType)
struct INVENTORY_API FInvItemManifest
{
	GENERATED_BODY()

public:
	UInvInventoryItem* Manifest(UObject* Outer) const;
	EInvItemCategory GetCategory() const { return Category; }
	const FGameplayTag& GetItemTag() const { return ItemTag; }
	void AssimilateInventoryFragments(UInvCompositeBase* Composite) const;

	template<typename T> requires std::derived_from<T, FInvItemFragment>
	const T* GetFragmentByTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FInvItemFragment>
	const T* GetFragment() const;
	
	template<typename T> requires std::derived_from<T, FInvItemFragment>
	T* GetFragmentMutable();

	template<typename T> requires std::derived_from<T, FInvItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

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

template<typename T> requires std::derived_from<T, FInvItemFragment>
const T* FInvItemManifest::GetFragment() const
{
	for (const TInstancedStruct<FInvItemFragment>& Fragment : Fragments)
	{
		if (const T* FoundFragment = Fragment.GetPtr<T>())
		{
			return FoundFragment; 
		}
	}	
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInvItemFragment>
T* FInvItemManifest::GetFragmentMutable()
{
	for (TInstancedStruct<FInvItemFragment>& Fragment : Fragments)
	{
		if (T* FoundFragment = Fragment.GetMutablePtr<T>())
		{
			return FoundFragment; 
		}
	}	
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInvItemFragment>
TArray<const T*> FInvItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FInvItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}
	return Result;
}
