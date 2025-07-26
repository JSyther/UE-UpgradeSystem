#include "UpgradeSystem.h"
#include "GameFramework/Actor.h"
#include "UObject/UnrealType.h" // Provides reflection utilities

UUpgradeSystem::UUpgradeSystem()
{
    // Constructor: Initialize any default upgrade data here if needed
}

EUpgradeLevel UUpgradeSystem::GetTargetCurrentLevel() const
{
    if (!TargetActor.IsValid())
    {
        // Target actor is invalid or has been destroyed
        return EUpgradeLevel::None;
    }

    // Attempt to locate the 'CurrentLevel' property on the target actor via reflection
    FProperty* Property = TargetActor->GetClass()->FindPropertyByName(FName("CurrentLevel"));
    if (!Property)
    {
        // Property not found; returning default level
        return EUpgradeLevel::None;
    }

    // Cast the property to an Enum property for proper handling
    FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property);
    if (!EnumProperty)
    {
        // The property exists but is not of enum type; cannot proceed
        return EUpgradeLevel::None;
    }

    // Obtain a pointer to the property's value within the target actor instance
    void* PropAddr = Property->ContainerPtrToValuePtr<void>(TargetActor.Get());
    if (!PropAddr)
    {
        // Invalid pointer to property value; aborting
        return EUpgradeLevel::None;
    }

    // Retrieve the underlying integer value of the enum and cast it back to EUpgradeLevel
    int64 EnumValue = EnumProperty->GetUnderlyingProperty()->GetSignedIntPropertyValue(PropAddr);
    return static_cast<EUpgradeLevel>(EnumValue);
}

bool UUpgradeSystem::SetTargetCurrentLevel(EUpgradeLevel NewLevel)
{
    if (!TargetActor.IsValid())
    {
        // Cannot set level on an invalid or destroyed actor
        return false;
    }

    // Locate the 'CurrentLevel' enum property on the target actor via reflection
    FProperty* Property = TargetActor->GetClass()->FindPropertyByName(FName("CurrentLevel"));
    if (!Property)
    {
        // Property not found; cannot set level
        return false;
    }

    FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property);
    if (!EnumProperty)
    {
        // Property is not an enum type; setting level not possible
        return false;
    }

    void* PropAddr = Property->ContainerPtrToValuePtr<void>(TargetActor.Get());
    if (!PropAddr)
    {
        // Invalid memory address for property; abort operation
        return false;
    }

    // Assign the new enum value to the property within the target actor instance
    EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(PropAddr, static_cast<int64>(NewLevel));
    return true;
}

bool UUpgradeSystem::IncreaseLevel()
{
    if (!TargetActor.IsValid())
    {
        // Target actor is not valid, cannot upgrade
        return false;
    }

    EUpgradeLevel CurrentLevel = GetTargetCurrentLevel();

    // Clamp the next level to the valid range to avoid exceeding the maximum level
    int32 NextLevelIndex = FMath::Clamp(static_cast<int32>(CurrentLevel) + 1, 
                                        static_cast<int32>(EUpgradeLevel::None), 
                                        static_cast<int32>(EUpgradeLevel::Legend));

    // If we are already at the max level, no upgrade occurs
    if (NextLevelIndex == static_cast<int32>(CurrentLevel))
    {
      // Already at the highest possible level
        return false; 
    }

    EUpgradeLevel NewLevel = static_cast<EUpgradeLevel>(NextLevelIndex);
    return SetTargetCurrentLevel(NewLevel);
}

bool UUpgradeSystem::DecreaseLevel()
{
    if (!TargetActor.IsValid())
    {
        // Target actor is invalid, cannot downgrade
        return false;
    }

    // Retrieve the current upgrade level of the target actor
    EUpgradeLevel CurrentLevel = GetTargetCurrentLevel();

    // Clamp the previous level to avoid going below the minimum level
    int32 PrevLevelIndex = FMath::Clamp(static_cast<int32>(CurrentLevel) - 1,
                                        static_cast<int32>(EUpgradeLevel::None),
                                        static_cast<int32>(EUpgradeLevel::Legend));

    // If no change in level (already at minimum), return false
    if (PrevLevelIndex == static_cast<int32>(CurrentLevel))
    {
        return false; // Already at lowest possible level
    }

    EUpgradeLevel NewLevel = static_cast<EUpgradeLevel>(PrevLevelIndex);

    // Update the target actor's level and return success status
    return SetTargetCurrentLevel(NewLevel);
}


FUpgradeData UUpgradeSystem::GetCurrentUpgradeData() const
{
    // Retrieve the current upgrade level of the target actor
    EUpgradeLevel CurrentLevel = GetTargetCurrentLevel();

    // Attempt to find upgrade data corresponding to the current level
    if (Upgrades.Contains(CurrentLevel))
    {
        return Upgrades[CurrentLevel];
    }

    // Return a default constructed FUpgradeData if no data exists for the current level
    return FUpgradeData();
}
