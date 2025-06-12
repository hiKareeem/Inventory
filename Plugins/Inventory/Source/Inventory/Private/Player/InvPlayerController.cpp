// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InvPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
