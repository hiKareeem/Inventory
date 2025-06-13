// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvItemManifest.h"
#include "Components/ActorComponent.h"
#include "InvItemComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInvItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInvItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FInvItemManifest GetItemManifest() const { return ItemManifest; }
	void PickedUp();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickedUp();

private:
	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
	FInvItemManifest ItemManifest;
	
};
