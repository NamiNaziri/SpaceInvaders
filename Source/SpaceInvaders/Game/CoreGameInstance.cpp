// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/BinarySearch.h"

#include "CoreSaveGame.h"

void UCoreGameInstance::Init()
{
	Super::Init();
	InitSaveFile();
}

void UCoreGameInstance::InitSaveFile()
{
	/* Try to load save data or create one if it doesn't exist. */
	bool bSaveGameExist = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	if (bSaveGameExist)
	{
		FAsyncLoadGameFromSlotDelegate AsyncLoadGameFromSlotDelegate;
		AsyncLoadGameFromSlotDelegate.BindUObject(this, &UCoreGameInstance::OnLoadGameFromSlotFinished);

		UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, AsyncLoadGameFromSlotDelegate);
	}
	else
	{
		TObjectPtr<USaveGame> NewSaveGame = UGameplayStatics::CreateSaveGameObject(UCoreSaveGame::StaticClass());
		if (NewSaveGame)
		{
			CoreSaveGame = Cast<UCoreSaveGame>(NewSaveGame);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Faild to create the save file"));
		}
	}
}

void UCoreGameInstance::AddToLeaderboard(FString PlayerName, float Score)
{
	FLeaderboardInfo NewInfo = FLeaderboardInfo(PlayerName, Score);

	// Binary seach to find index for the NewInfo. (We sort the leaderboard when loading from the save file.)
	int32 IndexToInsert = Algo::LowerBound(Leaderboard, FLeaderboardInfo(PlayerName, Score),
		[](const FLeaderboardInfo& Info1, const FLeaderboardInfo& Info2) {
			return  Info1.Score > Info2.Score;
		});

	Leaderboard.Insert(NewInfo, IndexToInsert);

	if (CoreSaveGame)
	{
		CoreSaveGame->SaveLeaderboard(SaveSlotName,FLeaderboardInfo(PlayerName, Score));
	}
}

const TArray<FLeaderboardInfo>& UCoreGameInstance::GetLeaderboard()
{
	return Leaderboard;
}

void UCoreGameInstance::OnLoadGameFromSlotFinished(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	if (SaveGame)
	{
		CoreSaveGame = Cast<UCoreSaveGame>(SaveGame);
		if (CoreSaveGame)
		{
			CoreSaveGame->LoadLeaderboard(this->Leaderboard);

			Leaderboard.Sort([](const FLeaderboardInfo& Info1, const FLeaderboardInfo& Info2) {
				return  Info1.Score > Info2.Score;
				});
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Faild to load the save file"));
	}
}
