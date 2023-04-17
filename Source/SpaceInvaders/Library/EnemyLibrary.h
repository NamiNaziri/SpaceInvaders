
#pragma once

#include "CoreMinimal.h"
#include "EnemyLibrary.generated.h"


UENUM(BlueprintType)
enum EMovementDirection
{
	Right UMETA(DisplayName = "Right"),
	Left UMETA(DisplayName = "Left"),
	Down UMETA(DisplayName = "Down")
};

UENUM(BlueprintType)
enum EEnemyMovementMode
{
	Normal UMETA(DisplayName = "Normal"),
	Freezed UMETA(DisplayName = "Freeze")
};

UENUM(BlueprintType)
enum EEnemySpawnMode
{
	Row_Random UMETA(DisplayName = "Row_Random"), // Chooses a class for each row randomly. (If you want the randomness be unform, you should only have a single item for each class in EnemyClass array. by having several item it's give that class more chance to be chosen)
	Individual_Random UMETA(DisplayName = "Individual_Random"), // Chooses a class for each place randomly. 
	Row_Sequencial UMETA(DisplayName = "Row_Sequencial"), // for row = i it chooses the EnemyClass[i]. So for spawner with 5 row, you should add 5 item to EnemyClass array.
};