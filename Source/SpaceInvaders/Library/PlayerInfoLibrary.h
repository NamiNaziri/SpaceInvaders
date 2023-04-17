
#pragma once

#include "CoreMinimal.h"
#include "PlayerInfoLibrary.generated.h"

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
