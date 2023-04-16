// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Delegates/Delegate.h"
#include "PoolableObjectInterface.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoolableObjectReleaseDelegate, AActor*, PooledActor);

UINTERFACE(MinimalAPI)
class UPoolableObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEINVADERS_API IPoolableObjectInterface
{
	GENERATED_BODY()

	//reference: https://forums.unrealengine.com/t/c-get-delegates-in-interface/438051/5

public:
	/*
	*	Every poolable objects should implmenet this function.
	*	The pool binds to this delegate. When the pooled object completes its task and needs to be returned to the pool, it can call this delegate to do so.
	*/
	virtual FOnPoolableObjectReleaseDelegate& GetPoolableObjectReleaseDelegate() = 0;
};
