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

UENUM()
enum class EInvTileQuadrant : uint8
{
	TopLeft UMETA(DisplayName = "Top Left"),
	TopRight UMETA(DisplayName = "Top Right"),
	BottomLeft UMETA(DisplayName = "Bottom Left"),
	BottomRight UMETA(DisplayName = "Bottom Right"),
	Invalid UMETA(DisplayName = "Invalid")
};

USTRUCT(BlueprintType)
struct FInvTileParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FIntPoint TileCoordinates;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 TileIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	EInvTileQuadrant Quadrant = EInvTileQuadrant::Invalid;
};

inline bool operator==(const FInvTileParameters& A, const FInvTileParameters& B)
{
	return A.TileCoordinates == B.TileCoordinates && A.TileIndex == B.TileIndex && A.Quadrant == B.Quadrant;
}

USTRUCT()
struct FInvSpaceQueryResult
{
	GENERATED_BODY()

public:
	bool bHasSpace = false;
	TWeakObjectPtr<UInvInventoryItem> ValidItem = nullptr;
	int32 UpperLeftIndex = INDEX_NONE;
};
