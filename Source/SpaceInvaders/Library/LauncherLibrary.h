
#pragma once

#include "CoreMinimal.h"
#include "LauncherLibrary.generated.h"

/*
*	TimerBased: The fire rate depends on timer. for example, player can shoot evert X seconds.
*	AvailabilityBased:  The fire rate depend on whether the projectile is available or not.
*						The projectile become avialable when the previous instanc has hit an object( can be enemy, enemy projectile, wall, etc)
*						It uses the poolobject bShouldCreateNew variable. The PoolCapacity usually is set to 1 in this option.
*/

UENUM(BlueprintType)
enum EFireRateMode
{
	TimerBased UMETA(DisplayName = "TimerBased"),
	AvailabilityBased UMETA(DisplayName = "AvailabilityBased")
};

