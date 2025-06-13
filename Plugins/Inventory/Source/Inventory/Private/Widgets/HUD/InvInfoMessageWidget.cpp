// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/InvInfoMessageWidget.h"

#include "Components/TextBlock.h"

void UInvInfoMessageWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	HideMessage();
}

void UInvInfoMessageWidget::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bMessageActive)
	{
		ShowMessage();
	}
	bMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]()
	{
		HideMessage();
		bMessageActive = false;
	}, MessageDuration, false);
}
