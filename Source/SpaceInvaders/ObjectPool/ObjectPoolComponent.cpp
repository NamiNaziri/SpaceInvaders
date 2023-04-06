// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolComponent.h"
#include "PoolableObjectInterface.h"
// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnObjects(AmountOfObjects);
	
}


// Called every frame
void UObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TObjectPtr<AActor> UObjectPoolComponent::GetNewObjectInstance()
{
	for (auto& [Object, bIsUsed] : PooledObjects)
	{
		if (!bIsUsed)
		{
			bIsUsed = true;

			Object->SetActorHiddenInGame(false);
			Object->SetActorEnableCollision(true);

			return Object;
		}
	}
	
	// if not found, create a newObj
	UE_LOG(LogTemp, Warning, TEXT("Object pool had shortage. A new object created."))
	TObjectPtr<AActor> newObj = SpawnSingleObject();
	PooledObjects[newObj] = true;

	newObj->SetActorHiddenInGame(false);
	newObj->SetActorEnableCollision(true);

	return newObj;
}

void UObjectPoolComponent::ReleaseObjectInstanceToPool(AActor* ObjectInstance)
{
	UE_LOG(LogTemp,Warning,TEXT("ReleaseObjectInstanceToPool"))
	ObjectInstance->SetActorHiddenInGame(true);
	ObjectInstance->SetActorEnableCollision(false);
	ObjectInstance->SetActorLocation(FVector(0.f, -1000.f, 0.f));
	PooledObjects[ObjectInstance] = false;

}

void UObjectPoolComponent::SpawnObjects(int num)
{
	for (int i = 0; i < num; i++)
	{
		SpawnSingleObject();
	}
}

TObjectPtr<AActor> UObjectPoolComponent::SpawnSingleObject()
{
	if (ClassToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();

		// Spawn the actor
		AActor* SpawnedObject = GetWorld()->SpawnActor<AActor>(ClassToSpawn, FVector(0.f, -1000.f, 0.f), FRotator(0.f), SpawnParams);

		SpawnedObject->SetActorHiddenInGame(true);
		SpawnedObject->SetActorEnableCollision(false);
		//SpawnedObject->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);

		IPoolableObjectInterface* PoolableObjectInterface = Cast<IPoolableObjectInterface>(SpawnedObject);
		if (PoolableObjectInterface)
		{
			FOnPoolableObjectReleaseDelegate* Delegate = &(PoolableObjectInterface->GetPoolableObjectReleaseDelegate());
			(*Delegate).AddDynamic(this, &UObjectPoolComponent::ReleaseObjectInstanceToPool);
		}
		


		PooledObjects.Emplace(SpawnedObject, false);

		return SpawnedObject;
	}
	return nullptr;
}

