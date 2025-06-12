#pragma once

#include "InvGridTypes.generated.h"

UENUM()
enum class EInvItemCategory : uint8
{
	Equippable UMETA(DisplayName = "Equippable"),
	None UMETA(DisplayName = "None")
};
