// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/InvLeaf.h"

void UInvLeaf::ApplyFunction(FuncType InFunction)
{
	InFunction(this);
}
