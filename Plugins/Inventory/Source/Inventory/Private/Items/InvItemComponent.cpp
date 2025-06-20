﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InvItemComponent.h"

#include "Net/UnrealNetwork.h"


UInvItemComponent::UInvItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInvItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInvItemComponent, ItemManifest);
}

void UInvItemComponent::PickedUp()
{
	OnPickedUp();	
	GetOwner()->Destroy();
}
