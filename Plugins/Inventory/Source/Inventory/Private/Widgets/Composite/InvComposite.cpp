// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/InvComposite.h"
#include "Blueprint/WidgetTree.h"

void UInvComposite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		UInvCompositeBase* Composite = Cast<UInvCompositeBase>(Widget);
		if (IsValid(Composite))
		{
			Children.Add(Composite);
			Composite->Collapse();
		}
	});
}

void UInvComposite::ApplyFunction(FuncType InFunction)
{
	for (UInvCompositeBase* Child : Children)
	{
		Child->ApplyFunction(InFunction);
	}
}

void UInvComposite::Collapse()
{
	for (UInvCompositeBase* Child : Children)
	{
		Child->Collapse();
	}
}
