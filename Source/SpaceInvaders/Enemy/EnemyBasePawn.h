// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "SpaceInvaders/Pawn/BasePawn.h"
#include "EnemyBasePawn.generated.h"

/* Delegate which called when the enemy is destroyed (its helath reaches zero) */
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyed, AEnemyBasePawn*, HitEnemy);

/*
*	Base class for enemies. 
*	They are called enemy and not AI, because on their own they do nothing. The Enemy Spawner manages the enemies.
*/
UCLASS()
class SPACEINVADERS_API AEnemyBasePawn : public ABasePawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnEnemyDestroyed OnEnemyDestroyed;

	const bool& IsEnemyEnable();
	void SetEnemyEnable(bool bIsEnabled);

	UFUNCTION()
		void Shoot();

	/* The use of the built-in reset function resulted in problems, notably the unintended destruction of actors. */
	void ResetEnemy();

protected:

	/* Points give to the player after destroying this enemy. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Points")
		float PointsPerKill = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Sound Cue")
		TObjectPtr<USoundBase> ExplosionSoundCue;

	/* 
	*	Last controller that damaged this enemy. 
	*	This is typically used in situations where the enemies have a maximum health greater than 1, and we need to determine which controller should be awarded the point.
	*	TODO: This can be moved to health component and return it using the FOnHealthBecomeZero delegate.
	*/
	UPROPERTY()
		AController* LastInstigator;

	/* When the actor is destroyed, it become disable. And becomes enable during the reset. It is used to determine new shooter in spawner class. */
	UPROPERTY()
		bool bIsEnemyEnabled = true;


	virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	/* Health component calls this function when the actor's health reaches zero*/
	virtual void HealthBecomeZero(AActor* OwnerActor) override;

	/* Overlap used for when the enemy overlaps the destructible actors.*/
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
