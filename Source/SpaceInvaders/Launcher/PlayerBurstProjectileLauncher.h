// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerProjectileLauncher.h"
#include "PlayerBurstProjectileLauncher.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API APlayerBurstProjectileLauncher : public APlayerProjectileLauncher
{
	GENERATED_BODY()

public:
	virtual void Shoot(FVector Location, FRotator Rotation, FVector LaunchDirection, float Speed = -1.f) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Launcher|Shooting")
		float Angle = 30.f;
};
