// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CoreGameInstance.h" // this should be removed
#include "CoreSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API UCoreSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	void SaveLeaderboard(FString SlotName, FLeaderboardInfo NewLeaderboardInfo);
	void LoadLeaderboard(TArray<FLeaderboardInfo>& LeaderboardToLoad);

protected:

	UPROPERTY()
		TArray<FLeaderboardInfo> Leaderboard;
};
