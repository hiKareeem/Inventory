// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvHUDWidget.generated.h"

class UInvInfoMessageWidget;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInvInfoMessageWidget> InfoMessageWidget;

	UFUNCTION()
	void OnInventoryFull();
};
