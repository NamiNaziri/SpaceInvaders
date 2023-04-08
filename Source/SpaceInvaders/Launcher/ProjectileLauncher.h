// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileLauncher.generated.h"

class UObjectPoolComponent;
class AProjectileBaseActor;


/*
*	This class has a pool of projectile base actor and can launch a projectile.
*/

UCLASS()
class SPACEINVADERS_API AProjectileLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Negative Speed uses the speed defined in the projectile component
	void Launch(FVector Location, FRotator Rotation, FVector LaunchDirection, float Speed = -1.f);

		TObjectPtr<UObjectPoolComponent> GetPoolComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UObjectPoolComponent> PoolComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Settings")
		//TSubclassOf<AProjectileBaseActor> ProjectileClass;
};
