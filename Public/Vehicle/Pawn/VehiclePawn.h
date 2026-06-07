// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "VehiclePawn.generated.h"

class UVehicleMovementComponent;
class USkeletalMeshComponent;
class UVehicleConfigData;


class UEnhancedInputComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class MYVEHICLEPROJECT_API AVehiclePawn : public APawn
{
	GENERATED_BODY()

public:
	AVehiclePawn();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	
	
public:	
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle Setup")
	TObjectPtr<UVehicleConfigData> VehicleConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> VehicleSkeletalMesh;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UVehicleMovementComponent> VehicleMovementComponent;

private:
	
	// Input
	void OnThrottleInput(const FInputActionValue& Value);
	void OnBrakeInput(const FInputActionValue& Value);
	void OnSteerInput(const FInputActionValue& Value);
	
};
