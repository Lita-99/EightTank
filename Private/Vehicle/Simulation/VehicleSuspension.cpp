#include "Vehicle/Simulation/VehicleTypes.h"


void FVehicleSuspension::Initialise(const FAxleSetup& InConfig)
{
	LoadCapacity = InConfig.AxleLoadCapacity / 2.f;
	RestCompressionLengthRatio = InConfig.RestCompressionLengthRatio;
	DampingRatio = InConfig.DampingRatio;
	TravelLength = InConfig.TravelLength;
	
	const float RestForce = LoadCapacity * 980.f;
	const float RestCompression = TravelLength * RestCompressionLengthRatio;
	
	Stiffness = RestForce / RestCompression;
	const float CriticalDampingForce = 2.f * FMath::Sqrt(Stiffness * LoadCapacity);
	
	Damping = CriticalDampingForce * DampingRatio;
}

void FVehicleSuspension::CalculateForces(const float DeltaTime)
{
	Compression = FMath::Max(0.0f, TravelLength - Length); 
	const float RawVelocity = (Length - PreviousLength) / DeltaTime;
	CompressionVelocity = FMath::Lerp(CompressionVelocity, RawVelocity, 0.2f);
	PreviousLength = Length;
	
	SpringForce = Compression * Stiffness;
	DampingForce = -(CompressionVelocity * Damping);
	
	AccumulatedForce = FMath::Max(
		SpringForce + DampingForce,
		0.f);
}