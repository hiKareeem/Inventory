// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/HUD/InvHUDWidget.h"

void AInvPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		for (UInputMappingContext* Context : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(Context, 0);
		}
	}

	ConstructHUD();
}

void AInvPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInvPlayerController::PrimaryInteract);
}

void AInvPlayerController::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("PrimaryInteract"));
}

void AInvPlayerController::ConstructHUD()
{
	if (!IsLocalController()) return;
	HUDWidget = CreateWidget<UInvHUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget)) HUDWidget->AddToViewport();
}
