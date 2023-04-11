// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "../Pawn/BasePawn.h"
#include "EnemyBasePawn.generated.h"


UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyed, AEnemyBasePawn*, HitEnemy);

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

	virtual void Reset() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Points)
		float PointsPerKill = 10.f;

	UPROPERTY()
		AController* LastInstigator;

	UPROPERTY()
		bool bIsEnemyEnabled = true;


	virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	virtual void HealthBecomeZero(AActor* OwnerActor) override;


};
