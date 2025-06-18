// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "InvCompositeBase.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInvCompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag& InFragmentTag) { FragmentTag = InFragmentTag; }

	virtual void Collapse();
	void Expand();

	using FuncType = TFunction<void(UInvCompositeBase*)>;
	virtual void ApplyFunction(FuncType InFunction) {}

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (Categories = "Fragments"))
	FGameplayTag FragmentTag;
};
