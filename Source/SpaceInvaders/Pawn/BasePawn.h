// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UBoxComponent;
class AProjectileBaseActor;
class AProjectileLauncher;
class UHealthComponent;

/*
*	Base class for player and enemiy pawn. 
*	It contains simple mesh with a box collider and health and projectile component.
*	It also contains a simple rotational animations that can be disabled. 
*	Now to disable the animations, the sub class should set the bDisableAnimation to true and also should disable the tick funcion in blueprint.
*	Since animation is the only functionality the uses the tick function, we will disable tick if the bDisableAnimation flag set to false.
*/

UCLASS()
class SPACEINVADERS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Shooting")
		TSubclassOf<AProjectileLauncher> ProjectileLauncherClass;

	UPROPERTY()
		TObjectPtr<AProjectileLauncher> ProjectileLauncher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Health")
		TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Visual|Particle")
		TObjectPtr<UParticleSystem> ExplosionParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Visual|Animation")
		bool bDisableAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Visual|Animation")
		float TargetAngleDifference = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Visual|Animation")
		float MeshRotationRate = 3.f;

	UPROPERTY()
		FVector PrevLocation;

	UPROPERTY()
		float InitMeshYaw;


	UFUNCTION()
		virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
		virtual void HealthBecomeZero(AActor* OwnerActor);

	virtual void InitProjectileLauncher();
};
