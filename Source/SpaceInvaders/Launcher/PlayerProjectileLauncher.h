#pragma once

#include "CoreMinimal.h"
#include "ProjectileLauncher.h"
#include "SpaceInvaders/Library/LauncherLibrary.h"
#include "PlayerProjectileLauncher.generated.h"

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
