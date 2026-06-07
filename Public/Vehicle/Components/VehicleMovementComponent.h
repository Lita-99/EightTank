// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle/Simulation/VehicleTypes.h"
#include "VehicleMovementComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYVEHICLEPROJECT_API UVehicleMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleMovementComponent();
	
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	
	bool InitialiseVehicle(
		USkeletalMeshComponent* InComponent,
		UVehicleConfigData* InConfigData);

	
	
	// Input
	void SetThrottle(float Value) { ThrottleInput = Value; }
	void SetBrake(float Value)    { BrakeInput = Value; }
	void SetSteer(float Value)    { SteerInput = Value; }
	
private:
	// Components
	FBodyInstance* BodyInstance;
	UPROPERTY()
	TArray<UStaticMeshComponent*> WheelMeshes;
	
	
	// Runtime data and state
	float ThrottleInput;
	float BrakeInput;
	float SteerInput;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle", meta=(AllowPrivateAccess="true"))
	FVehicleBase Vehicle;
};

