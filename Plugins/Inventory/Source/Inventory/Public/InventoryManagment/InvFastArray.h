#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InvFastArray.generated.h"

struct FGameplayTag;
class UInvItemComponent;
struct FInvInventoryFastArray;
class UInvInventoryComponent;
class UInvInventoryItem;

USTRUCT(BlueprintType)
struct FInvInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FInvInventoryEntry() {}

private:
	friend UInvInventoryComponent;
	friend FInvInventoryFastArray;
	
	UPROPERTY()
	TObjectPtr<UInvInventoryItem> Item = nullptr;
};

USTRUCT()
struct FInvInventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FInvInventoryFastArray() : OwningComponent(nullptr) {}
	explicit FInvInventoryFastArray(UActorComponent* InOwningComponent) : OwningComponent(InOwningComponent) {}

	TArray<TObjectPtr<UInvInventoryItem>> GetAllItems() const;

	// FFastArraySerializer
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	// End FFastArraySerializer

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInvInventoryEntry, FInvInventoryFastArray>(Entries, DeltaParams, *this);
	}

	UInvInventoryItem* AddEntry(UInvItemComponent* ItemComponent);
	UInvInventoryItem* AddEntry(UInvInventoryItem* Item);
	void RemoveEntry(UInvInventoryItem* Item);

	UInvInventoryItem* FindFirstItemByTag(const FGameplayTag& Tag) const;

private:
	friend UInvInventoryComponent;

	UPROPERTY()
	TArray<FInvInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwningComponent;
};

template<>
struct TStructOpsTypeTraits<FInvInventoryFastArray> : public TStructOpsTypeTraitsBase2<FInvInventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};