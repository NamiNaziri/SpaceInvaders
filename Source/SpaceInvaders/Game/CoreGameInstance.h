// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoreGameInstance.generated.h"



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

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API UCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddToLeaderboard(FString PlayerName, float Score);

	UFUNCTION(BlueprintCallable)
	const TArray<FLeaderboardInfo>& GetLeaderboard();


protected:

	TArray<FLeaderboardInfo> Leaderboard;
};
