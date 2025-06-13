#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InvGridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "InvItemManifest.generated.h"

USTRUCT(BlueprintType)
struct INVENTORY_API FInvItemManifest
{
	GENERATED_BODY()

public:
	UInvInventoryItem* Manifest(UObject* Outer) const;
	EInvItemCategory GetCategory() const { return Category; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	EInvItemCategory Category = EInvItemCategory::None;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (Categories = "GameItems"))
	FGameplayTag ItemTag;
};
