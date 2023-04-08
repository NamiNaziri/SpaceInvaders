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

#include "../Launcher/ProjectileLauncher.h"

// Sets default values
APlayerBasePawn::APlayerBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	SetRootComponent(BoxComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerBasePawn::OnBoxBeginOverlap);

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Pawn Movement"));
	MovementComponent->UpdatedComponent = BoxComponent;

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

	InitProjectileLauncher();
	
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

	if (!ProjectileLauncher)
		return;

	FVector ProjectileLocation = GetActorLocation() + GetActorUpVector() * BoxComponent->GetScaledBoxExtent().Z;
	ProjectileLauncher->Launch(ProjectileLocation, GetActorRotation(), GetActorUpVector(), -1.f);
	
}

void APlayerBasePawn::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APlayerBasePawn::InitProjectileLauncher()
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// Spawn the actor
	ProjectileLauncher = GetWorld()->SpawnActor<AProjectileLauncher>(
		ProjectileLauncherClass,
		GetActorLocation(),
		GetActorRotation(),
		SpawnParams);
	if(ProjectileLauncher)
		ProjectileLauncher->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

