// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UCoreSaveGame::SaveLeaderboard(FString SlotName, FLeaderboardInfo NewLeaderboardInfo)
{
	Leaderboard.Push(NewLeaderboardInfo);
	const bool IsSaved = UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
	if (!IsSaved)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Saved"));
	}
}

void UCoreSaveGame::LoadLeaderboard(TArray<FLeaderboardInfo>& LeaderboardToLoad)
{
	LeaderboardToLoad = Leaderboard;
}
