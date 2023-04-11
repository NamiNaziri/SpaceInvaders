// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameInstance.h"
#include "Kismet/GameplayStatics.h"
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
	Leaderboard.Push(FLeaderboardInfo(PlayerName, Score));
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
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Faild to load the save file"));
	}
}
