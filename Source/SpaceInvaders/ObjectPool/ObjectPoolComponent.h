// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEINVADERS_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TObjectPtr<AActor> GetNewObjectInstance();

	UFUNCTION()
	void ReleaseObjectInstanceToPool(AActor* ObjectInstance);

	UFUNCTION()
		void SetShouldCreateNew(bool ShouldCreateNew);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Settings")
		TSubclassOf<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Settings")
		int PoolCapacity = 1;

	/*
	* Should create new object if there is no remaining object in the pool.
	* if false will return nullptr when asking for new object and the pool is empty
	*/
	UPROPERTY()
		bool bShouldCreateNew = true;

	// A map between actors and a bool. the boolean shows whether the actor is currently used or not.
	UPROPERTY()
		TMap<TObjectPtr<AActor>, bool> PooledObjects;


	UFUNCTION()
	void SpawnObjects(int num);
	TObjectPtr<AActor> SpawnSingleObject();
};
