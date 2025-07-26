#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpgradeLevel.h"
#include "UpgradeSystem.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
    EUpgradeLevel Level = EUpgradeLevel::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
    float BonusValue = 0.f;
};

UCLASS(Blueprintable, BlueprintType)
class YOURGAME_API UUpgradeSystem : public UObject
{
    GENERATED_BODY()

public:
    UUpgradeSystem();

    /** Target Actor to upgrade */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
    TWeakObjectPtr<AActor> TargetActor;

    /** Upgrade data for levels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
    TMap<EUpgradeLevel, FUpgradeData> Upgrades;

    /** Attempt to increase upgrade level on target actor */
    UFUNCTION(BlueprintCallable, Category="Upgrade")
    bool IncreaseLevel();

    /** Attempt to decrease upgrade level on target actor */
    UFUNCTION(BlueprintCallable, Category="Upgrade")
    bool DecreaseLevel();

    /** Get current upgrade data from target */
    UFUNCTION(BlueprintCallable, Category="Upgrade")
    FUpgradeData GetCurrentUpgradeData() const;

protected:
    /** Helper to get current level from target actor */
    EUpgradeLevel GetTargetCurrentLevel() const;

    /** Helper to set current level on target actor */
    bool SetTargetCurrentLevel(EUpgradeLevel NewLevel);
};
