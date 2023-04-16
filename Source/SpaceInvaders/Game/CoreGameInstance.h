// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoreGameInstance.generated.h"

// saving functionality is adopted from this toturial: https://www.youtube.com/watch?v=UsHuonij43o

class UCoreSaveGame;
class USaveGame;

USTRUCT(BlueprintType)
struct FLeaderboardInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeaderboardInfo")
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeaderboardInfo")
		float Score;

	FLeaderboardInfo() {}

	FLeaderboardInfo(FString PlayerName, float Score) :
		PlayerName(PlayerName),
		Score(Score)
	{}
};

/*
 * Game Instance manages the leaderboard and saving data between sessions by using the CoreSaveGame class. 
 */
UCLASS()
class SPACEINVADERS_API UCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	void InitSaveFile();

	UFUNCTION(BlueprintCallable)
		void AddToLeaderboard(FString PlayerName, float Score);

	UFUNCTION(BlueprintCallable)
		const TArray<FLeaderboardInfo>& GetLeaderboard();

protected:

	UPROPERTY()
		TArray<FLeaderboardInfo> Leaderboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), category = "Save")
		FString SaveSlotName = "GameSlot0";

	UPROPERTY()
		TObjectPtr<UCoreSaveGame> CoreSaveGame;


	void OnLoadGameFromSlotFinished(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);

	
};
