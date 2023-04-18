// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectileLauncher.h"

#include "Kismet/GameplayStatics.h"
#include "SpaceInvaders/ObjectPool/ObjectPoolComponent.h"

APlayerProjectileLauncher::APlayerProjectileLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCanShoot = true;
}

void APlayerProjectileLauncher::BeginPlay()
{
	Super::BeginPlay();

	/* When using a projectile-based firing rate, the object pool should not create new objects once it has run out of available ones. */
	if (FireRateMode == EFireRateMode::AvailabilityBased)
	{
		PoolComponent->SetShouldCreateNew(false);
	}
}


void APlayerProjectileLauncher::CanShoot()
{
	bCanShoot = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_CanShoot);


}

void APlayerProjectileLauncher::Shoot(FVector Location, FRotator Rotation, FVector LaunchDirection, float Speed)
{
	if (FireRateMode == EFireRateMode::TimerBased && bCanShoot)
	{
		bCanShoot = false;

		Launch(Location, Rotation, LaunchDirection, -1.f);

		FTimerDelegate TimerDelagate = FTimerDelegate::CreateUObject(this, &APlayerProjectileLauncher::CanShoot);
		GetWorldTimerManager().SetTimer(
			TimerHandle_CanShoot,
			TimerDelagate,
			FireRate,
			false);
	}
	else if (FireRateMode == EFireRateMode::AvailabilityBased)
	{
		bool bSuccess = Launch(Location, Rotation, LaunchDirection, -1.f);
		if (bSuccess)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ShootingSoundCue);
		}
	}
}

