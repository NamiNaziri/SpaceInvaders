// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileLauncher.generated.h"

class UObjectPoolComponent;
class AProjectileBaseActor;

/*
*	This class has a pool of projectile base actors and can launch a projectile.
*	This is a base class for projectile luncher that can only lunch a single projectile.
*/

UCLASS()
class SPACEINVADERS_API AProjectileLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileLauncher(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	*	Gets a single projectile from the pool and launches toward the give direction
	*	Negative Speed uses the speed defined in the projectile component
	*/
	bool Launch(FVector Location, FRotator Rotation, FVector LaunchDirection, float Speed = -1.f);

	TObjectPtr<UObjectPoolComponent> GetPoolComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UObjectPoolComponent> PoolComponent;

};
