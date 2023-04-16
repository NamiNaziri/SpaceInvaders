
#include "SpawnLocationActor.h"

// Sets default values
ASpawnLocationActor::ASpawnLocationActor()
{
 	// We don't need tick
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawnLocationActor::BeginPlay()
{
	Super::BeginPlay();
	
}
