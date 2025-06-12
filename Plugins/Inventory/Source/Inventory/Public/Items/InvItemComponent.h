// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InvItemComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInvItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInvItemComponent();
};
