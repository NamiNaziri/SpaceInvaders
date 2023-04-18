// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBurstProjectileLauncher.h"
#include <Kismet/KismetMathLibrary.h>

void APlayerBurstProjectileLauncher::Shoot(FVector Location, FRotator Rotation, FVector LaunchDirection, float Speed)
{
	if (FireRateMode == EFireRateMode::TimerBased && bCanShoot)
	{
		bCanShoot = false;

		FVector RightLaunchDirection = LaunchDirection.RotateAngleAxis(Angle, GetActorForwardVector());
		FVector LeftLaunchDirection = LaunchDirection.RotateAngleAxis(-Angle, GetActorForwardVector());
			
		Launch(Location, Rotation, LaunchDirection, -1.f);
		Launch(Location, UKismetMathLibrary::MakeRotFromZ(RightLaunchDirection), RightLaunchDirection, -1.f);
		Launch(Location, UKismetMathLibrary::MakeRotFromZ(LeftLaunchDirection), LeftLaunchDirection, -1.f);

		FTimerDelegate TimerDelagate = FTimerDelegate::CreateUObject(this, &APlayerBurstProjectileLauncher::CanShoot);
		GetWorldTimerManager().SetTimer(
			TimerHandle_CanShoot,
			TimerDelagate,
			FireRate,
			false);
	}
}

void APlayerBurstProjectileLauncher::BeginPlay()
{
	Super::BeginPlay();
}
