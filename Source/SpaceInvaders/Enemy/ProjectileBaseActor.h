// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ObjectPool/PoolableObjectInterface.h"
#include "Delegates/Delegate.h"
#include "ProjectileBaseActor.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class SPACEINVADERS_API AProjectileBaseActor : public AActor, public IPoolableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Damage = 1.f;



	UPROPERTY()
		FOnPoolableObjectReleaseDelegate PoolableObjectReleaseDelegate;

	UFUNCTION()
	virtual FOnPoolableObjectReleaseDelegate& GetPoolableObjectReleaseDelegate() override;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
