// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActor.generated.h"

class UGeometryCollectionComponent;
class UHealthComponent;
class UBoxComponent;
class AFieldSystemActor;

/*
*	Base class for destructible objects. 
*	The buckers use this class. It is important that the anchors being placed in the scene by designers. 
*	Currently it wasn't possible to create them during the onConstruct. I had problem with dupplication.
*/

UCLASS()
class SPACEINVADERS_API ADestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ADestructibleActor();
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	float GetRemainingHealth();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Destructible")
		TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> BoxComponent;

	/* Blueprint sets this*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Destructible|FieldSystem")
		TObjectPtr<AFieldSystemActor> AnchorField;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Destructible|FieldSystem")
		TSubclassOf<AFieldSystemActor> SelfDestructionDamageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Destructible")
		TObjectPtr<UParticleSystem> DestructionParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Destructible|Sound Cue")
		TObjectPtr<USoundBase> DebrisFallingSoundCue;

	UFUNCTION()
		virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
		virtual void HealthBecomeZero(AActor* OwnerActor);

	UPROPERTY()
		float BoxAdjustmentOffset;
};
