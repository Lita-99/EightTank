#include "Vehicle/Simulation/VehicleTypes.h"
#include "Vehicle/Data/WheelConfigData.h"

void FVehicleWheel::Initialise(const UWheelConfigData* InConfig)
{
	Radius = InConfig->Radius;
	Mass = InConfig->Mass;
	LateralGripFactor = InConfig->LateralGripFactor;
}

void FVehicleWheel::Update(const float DeltaTime)
{
	AngularVelocity = LongitudinalVelocity / Radius;
	CumulativeRotation += AngularVelocity * DeltaTime;
}