#pragma once

#include "Engine/SkeletalMeshSocket.h"
#include "Vehicle/Simulation/VehicleTypes.h"
#include "VehicleConfigData.generated.h"

UCLASS(BlueprintType)
class UVehicleConfigData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle Setup")
	FName ChassisBoneName = FName("Hull");
	
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle Setup")
	TObjectPtr<UPhysicsAsset> PhysicsAssetOverride;
	
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle Setup")
	float MaxDriveForce = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle Setup")
	float DragCoefficient = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Axle")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel = ECC_GameTraceChannel1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Axle")
	TArray<FAxleSetup> AxleSetups;


	UFUNCTION()
	TArray<FName> GetSocketOptions() const
	{
		TArray<FName> Names;
		if (!SkeletalMesh) return Names;

		for (const USkeletalMeshSocket* Socket : SkeletalMesh->GetActiveSocketList())
		{
			Names.Add(Socket->SocketName);
		}
		return Names;
	}
	
	
};