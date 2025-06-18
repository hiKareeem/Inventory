// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvCompositeBase.h"
#include "InvComposite.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInvComposite : public UInvCompositeBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void ApplyFunction(FuncType InFunction) override;
	virtual void Collapse() override;

private:
	TArray<TObjectPtr<UInvCompositeBase>> Children;
};
