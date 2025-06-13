#pragma once

#include "InvGridTypes.generated.h"

class UInvInventoryItem;

UENUM()
enum class EInvItemCategory : uint8
{
	Equippable UMETA(DisplayName = "Equippable"),
	None UMETA(DisplayName = "None")
};

USTRUCT()
struct FInvSlotAvailability
{
	GENERATED_BODY()

public:
	FInvSlotAvailability() {}
	FInvSlotAvailability(int32 InIndex, int32 InAmountToFill, bool bInItemAtIndex) : Index(InIndex), AmountToFill(InAmountToFill), bItemAtIndex(bInItemAtIndex)	{}

	int32 Index = INDEX_NONE;
	int32 AmountToFill = 0;
	bool bItemAtIndex = false;
};

USTRUCT()
struct FInvSlotAvailabilityResult
{
	GENERATED_BODY()

public:
	FInvSlotAvailabilityResult() {}

	TWeakObjectPtr<UInvInventoryItem> Item;
	int32 TotalRoomToFill = 0;
	int32 Remainder = 0;
	bool bStackable = false;
	TArray<FInvSlotAvailability> SlotAvailabilities;
};
