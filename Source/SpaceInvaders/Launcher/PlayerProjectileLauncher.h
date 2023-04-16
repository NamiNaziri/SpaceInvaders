
#pragma once

#include "CoreMinimal.h"
#include "ProjectileLauncher.h"
#include "PlayerProjectileLauncher.generated.h"

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


/**
 * Base class for player projectile that has the shooting functionality.
 * We can create different projectile type by overwriting the shooting functionality.
 */
UCLASS()
class SPACEINVADERS_API APlayerProjectileLauncher : public AProjectileLauncher
{
	GENERATED_BODY()

public:
	APlayerProjectileLauncher(const FObjectInitializer& ObjectInitializer);

	virtual void Shoot(FVector Location, FRotator Rotation, FVector LaunchDirection, float Speed = -1.f);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Launcher|Shooting")
		TEnumAsByte<EFireRateMode> FireRateMode = EFireRateMode::AvailabilityBased;

	/* Rate at which a weapon can fire projectiles. 1 shot per FireRate second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Player|Shooting")
		float FireRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Player|Sound Cue")
		TObjectPtr<USoundBase> ShootingSoundCue;

	/* Timer used for handling fire rate and TimerBased EFireRateMode*/
	FTimerHandle TimerHandle_CanShoot;

	bool bCanShoot = true;

	UFUNCTION()
		void CanShoot();

};
