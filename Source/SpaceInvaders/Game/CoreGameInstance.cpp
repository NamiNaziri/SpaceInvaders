// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameInstance.h"

void UCoreGameInstance::AddToLeaderboard(FString PlayerName, float Score)
{
	Leaderboard.Push(FLeaderboardInfo(PlayerName, Score));
}

const TArray<FLeaderboardInfo>& UCoreGameInstance::GetLeaderboard()
{

	return Leaderboard;
}
