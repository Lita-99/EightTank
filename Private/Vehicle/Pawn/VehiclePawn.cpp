// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/Pawn/VehiclePawn.h"
#include "Vehicle/Components/VehicleMovementComponent.h"
#include "Vehicle/Simulation/VehicleInputSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	VehicleSkeletalMesh = 
		CreateDefaultSubobject<USkeletalMeshComponent>("VehicleSkeletalMesh");
	SetRootComponent(VehicleSkeletalMesh);
	VehicleSkeletalMesh->SetSimulatePhysics(false);
	VehicleMovementComponent = 
		CreateDefaultSubobject<UVehicleMovementComponent>(TEXT("VehicleMovementComponent"));
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	
	
	
	if (IsValid(VehicleMovementComponent))
	{
		if (!VehicleMovementComponent->InitialiseVehicle(VehicleSkeletalMesh, VehicleConfig))
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]: Initialise failed!"), *GetName());
		}
	}
}

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}


// Called when pawn possessed
void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	const UVehicleInputSettings* Settings = GetDefault<UVehicleInputSettings>();
	if (!Settings) return;

	// Apply Mapping Context
	if (ULocalPlayer* LP = PC->GetLocalPlayer())
	{
		if (auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!Settings->DefaultMappingContext.IsNull())
			{
				Subsystem->AddMappingContext(Settings->DefaultMappingContext.LoadSynchronous(), 0);
			}
		}
	}

	// Bind inputs
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (UInputAction* Throttle = Settings->ThrottleAction.LoadSynchronous())
		{
			EnhancedInputComp->BindAction(Throttle, ETriggerEvent::Triggered, this, &AVehiclePawn::OnThrottleInput);
			EnhancedInputComp->BindAction(Throttle, ETriggerEvent::Completed, this, &AVehiclePawn::OnThrottleInput);
		}

		if (UInputAction* Brake = Settings->BrakeAction.LoadSynchronous())
		{
			EnhancedInputComp->BindAction(Brake, ETriggerEvent::Triggered, this, &AVehiclePawn::OnBrakeInput);
			EnhancedInputComp->BindAction(Brake, ETriggerEvent::Completed, this, &AVehiclePawn::OnBrakeInput);
		}

		if (UInputAction* Steer = Settings->SteerAction.LoadSynchronous())
		{
			EnhancedInputComp->BindAction(Steer, ETriggerEvent::Triggered, this, &AVehiclePawn::OnSteerInput);
			EnhancedInputComp->BindAction(Steer, ETriggerEvent::Completed, this, &AVehiclePawn::OnSteerInput);
		}
	}
}

void AVehiclePawn::OnThrottleInput(const FInputActionValue& Value)
{
	if (IsValid(VehicleMovementComponent))
	{
		VehicleMovementComponent->SetThrottle(Value.Get<float>());
	}
}

void AVehiclePawn::OnBrakeInput(const FInputActionValue& Value)
{
	if (IsValid(VehicleMovementComponent))
	{
		VehicleMovementComponent->SetBrake(Value.Get<float>());
	}
}

void AVehiclePawn::OnSteerInput(const FInputActionValue& Value)
{
	if (IsValid(VehicleMovementComponent))
	{
		VehicleMovementComponent->SetSteer(Value.Get<float>());
	}
}

