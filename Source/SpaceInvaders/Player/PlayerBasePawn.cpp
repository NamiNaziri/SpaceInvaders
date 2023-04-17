// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBasePawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/GameEngine.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "SpaceInvaders/Launcher/ProjectileBaseActor.h"
#include "SpaceInvaders/Launcher/ProjectileLauncher.h"
#include "SpaceInvaders/ObjectPool/ObjectPoolComponent.h"
#include "SpaceInvaders/GameComponents/Health/HealthComponent.h"
#include "SpaceInvaders/Launcher/PlayerProjectileLauncher.h"
#include "PlayerBaseController.h"

// Sets default values
APlayerBasePawn::APlayerBasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Pawn Movement"));
	MovementComponent->UpdatedComponent = BoxComponent;
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

	APlayerBaseController* PBC = GetController<APlayerBaseController>();
	PBC->SetMaxHealth(HealthComponent->GetMaxHealth());
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

void APlayerBasePawn::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Player took damage"));

	HealthComponent->DecreaseHealth(Damage);

	/* Notify the player state of the updated health. */
	TObjectPtr<APlayerBaseController> PBC = GetController<APlayerBaseController>();
	if (PBC)
	{
		PBC->SetHealth(HealthComponent->GetCurrentHealth());
	}

	if (ImpactSoundCue)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ImpactSoundCue);
	}
}

void APlayerBasePawn::HealthBecomeZero(AActor* OwnerActor)
{
	Super::HealthBecomeZero(OwnerActor);

	/* Controller handles player's death*/
	APlayerBaseController* PBC = GetController<APlayerBaseController>();
	if (PBC)
	{
		PBC->RecieveOnDeath();
	}
}

void APlayerBasePawn::Move(const FInputActionInstance& Instance)
{
	FVector2D MoveInput = Instance.GetValue().Get<FVector2D>();

	AddMovementInput(GetActorRightVector(), -1 * MoveInput.X * MovementSpeed);
}

void APlayerBasePawn::Shoot(const FInputActionInstance& Instance)
{
	FVector ProjectileLocation = GetActorLocation() + GetActorUpVector() * BoxComponent->GetScaledBoxExtent().Z;
	if (PlayerProjectileLauncher)
	{
		PlayerProjectileLauncher->Shoot(ProjectileLocation, GetActorRotation(), GetActorUpVector(), -1.f);
	}
}


void APlayerBasePawn::InitProjectileLauncher()
{
	Super::InitProjectileLauncher();

	if (ProjectileLauncher)
	{
		PlayerProjectileLauncher = Cast<APlayerProjectileLauncher>(ProjectileLauncher);
		if (!PlayerProjectileLauncher)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player projectile launcher not found."));
		}
	}
}

