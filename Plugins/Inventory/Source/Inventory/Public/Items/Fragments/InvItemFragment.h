#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InvItemFragment.generated.h"

USTRUCT(BlueprintType)
struct INVENTORY_API FInvItemFragment
{
	GENERATED_BODY()

	FInvItemFragment() {}

	FInvItemFragment(const FInvItemFragment& Other) = default;
	FInvItemFragment& operator=(const FInvItemFragment& Other) = default;
	FInvItemFragment(FInvItemFragment&& Other) = default;
	FInvItemFragment& operator=(FInvItemFragment&& Other) = default;
	virtual ~FInvItemFragment() {}

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag InFragmentTag) { FragmentTag = InFragmentTag; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (Categories = "Fragments"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FInvGridFragment : public FInvItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(const FIntPoint InGridSize) { GridSize = InGridSize; }
	
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(const float InGridPadding) { GridPadding = InGridPadding; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FIntPoint GridSize = FIntPoint(1, 1);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float GridPadding = 2.f;
};

USTRUCT(BlueprintType)
struct FInvImageFragment : public FInvItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetImage() const { return Image; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UTexture2D> Image = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FVector2D ImageSize = FVector2D(100.f, 100.f);
};

USTRUCT(BlueprintType)
struct FInvStackableFragment : public FInvItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStack() const { return MaxStack; }
	int32 GetCurrentStack() const { return CurrentStack; }
	void SetCurrentStack(int32 InCurrentStack) { CurrentStack = InCurrentStack; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 MaxStack = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 CurrentStack = 1;
};