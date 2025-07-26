#pragma once

#include "CoreMinimal.h"
#include "UpgradeLevel.generated.h"

/**
 * EUpgradeLevel
 * 
 * Defines the upgrade progression levels.
 */
UENUM(BlueprintType)
enum class EUpgradeLevel : uint8
{
    None        UMETA(DisplayName = "None"),
    Basic       UMETA(DisplayName = "Basic"),
    Advanced    UMETA(DisplayName = "Advanced"),
    Elite       UMETA(DisplayName = "Elite"),
    Master      UMETA(DisplayName = "Master"),
    Legend      UMETA(DisplayName = "Legend")
};
