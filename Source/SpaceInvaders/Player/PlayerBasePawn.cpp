// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBasePawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/GameEngine.h"

#include <Components/BoxComponent.h>

#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/FloatingPawnMovement.h>

#include "../Enemy/ProjectileBaseActor.h"

// Sets default values
APlayerBasePawn::APlayerBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	SetRootComponent(BoxComponent);

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Pawn Movement"));
	MovementComponent->UpdatedComponent = BoxComponent;


	//TODO
	//BoxComponent->SetCollisionProfileName("OverlapProfile");
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::LeftBoxOnOverlapBegin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerBasePawn::BeginPlay()
{
	Super::BeginPlay();


	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Default_KBM_MappingContext, 0);
		}
	}
	
}

// Called every frame
void APlayerBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerBasePawn::Move);

	Input->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &APlayerBasePawn::Shoot);
}

void APlayerBasePawn::Move(const FInputActionInstance& Instance)
{

	FVector2D MoveInput = Instance.GetValue().Get<FVector2D>();


	AddMovementInput(GetActorRightVector(), -1 * MoveInput.X * MovementSpeed);

}

void APlayerBasePawn::Shoot(const FInputActionInstance& Instance)
{
	bool bShoot = Instance.GetValue().Get<bool>();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FVector SpawnLocation = GetActorLocation() + GetActorUpVector() * BoxComponent->GetScaledBoxExtent().Z;

	// Spawn the actor
	AProjectileBaseActor* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBaseActor>(ProjectileType, SpawnLocation, GetActorRotation(), SpawnParams);


}

