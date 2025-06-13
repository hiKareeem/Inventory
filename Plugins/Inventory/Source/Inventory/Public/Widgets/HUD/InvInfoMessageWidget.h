// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvInfoMessageWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInvInfoMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	void SetMessage(const FText& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowMessage();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void HideMessage();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float MessageDuration = 1.f;

	FTimerHandle MessageTimer;
	bool bMessageActive = false;
};
