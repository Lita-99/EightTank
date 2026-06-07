#pragma once


#include "WheelConfigData.generated.h"

UCLASS(BlueprintType)
class MYVEHICLEPROJECT_API UWheelConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Radius = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Mass = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float LateralGripFactor = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMesh> WheelMesh;
};