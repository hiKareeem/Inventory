// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InvPlayerController.generated.h"

class UInvHUDWidget;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class INVENTORY_API AInvPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void PrimaryInteract();
	void ConstructHUD();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Input")
	TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Input")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|UI")
	TSubclassOf<UInvHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInvHUDWidget> HUDWidget;
};
