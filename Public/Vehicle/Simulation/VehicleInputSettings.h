#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "VehicleInputSettings.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Vehicle Input Settings"))
class MYVEHICLEPROJECT_API UVehicleInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Global Input Actions")
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(Config, EditAnywhere, Category = "Global Input Actions")
	TSoftObjectPtr<UInputAction> ThrottleAction;

	UPROPERTY(Config, EditAnywhere, Category = "Global Input Actions")
	TSoftObjectPtr<UInputAction> BrakeAction;

	UPROPERTY(Config, EditAnywhere, Category = "Global Input Actions")
	TSoftObjectPtr<UInputAction> SteerAction;
	
};