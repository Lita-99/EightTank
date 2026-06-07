#include "Vehicle/Simulation/VehicleTypes.h"
#include "Vehicle/Data/VehicleConfigData.h"


void FVehicleBase::Initialise(
	const USkeletalMeshComponent* InComponent,
	UVehicleConfigData* InConfigData)
{
	DragCoefficient = InConfigData->DragCoefficient;
	MaxDriveForce = InConfigData->MaxDriveForce;
	
	// Initialise Axles
	Axles.Empty();
	
	for (FAxleSetup& Setup : InConfigData->AxleSetups)
	{
		FVehicleAxle& Axle = Axles.AddDefaulted_GetRef();
		
		Axle.Left.LocalTransform  = InComponent->GetSocketTransform(Setup.LeftWheelSocket,  RTS_Component);
		Axle.Right.LocalTransform = InComponent->GetSocketTransform(Setup.RightWheelSocket, RTS_Component);
		
		Axle.Initialise(Setup);
	}
	
	// Sort Axles
	for (int32 i = 0; i < Axles.Num(); i++)
	{
		if (Axles[i].bCanSteer)  SteerableAxleIndices.Add(i);
		if (!Axles[i].bCanSteer)	NonSteerableAxleIndices.Add(i);
		if (Axles[i].bDriven)    DrivenAxleIndices.Add(i);
	}
	
	

	// Define how to determine vehicle pivot source
	const TArray<int32>& PivotSource = NonSteerableAxleIndices.Num() > 0
		? NonSteerableAxleIndices // Use Non-steerable as pivot
		: SteerableAxleIndices; // All steer, calculate pivot source from all axles

	// Determine Pivot X vector location
	float PivotX = 0.f;
	for (int32 i : PivotSource)
	{
		// Sum all X vector values of axles
		PivotX += Axles[i].LongitudinalPosition;
	}
	
	// Divide the vector by non-steerable axle count (or all axles if all-steer)
	PivotX /= PivotSource.Num();
	
	
	// Set reference offset and max angle
	for (int32 i : SteerableAxleIndices)
	{
		Axles[i].LongitudinalPivotOffset =
			Axles[i].LongitudinalPosition - PivotX;
		
		float Offset = FMath::Abs(Axles[i].LongitudinalPivotOffset);
		if (Offset > AckermannRefOffset)		
		{
			AckermannRefOffset   = Offset;
			AckermannRefMaxAngle = Axles[i].MaxSteerAngle;

			AckermannPivotX = PivotX;
		}
	}
}

void FVehicleBase::LongitudinalForce(const float DeltaTime, FBodyInstance* BodyInstance)
{
	const FVector Forward = Transform.GetUnitAxis(EAxis::X);
	VehicleSpeed = FVector::DotProduct(
		BodyInstance->GetUnrealWorldVelocity(), Forward);

	DriveForce = MaxDriveForce * Throttle;
	BleedForce = -VehicleSpeed * DragCoefficient;
}



void FVehicleBase::UpdateSteering()
{
	if (FMath::Abs(Steer) < KINDA_SMALL_NUMBER)
	{
		for (const int32 i : SteerableAxleIndices) Axles[i].ResetSteering();
		return;
	}

	for (const int32 i : SteerableAxleIndices)
	{
		const float Angle = FMath::DegreesToRadians(Steer * Axles[i].MaxSteerAngle);
		Axles[i].Left.Wheel.SteerAngleRad  = Angle;
		Axles[i].Right.Wheel.SteerAngleRad = Angle;
	}
}
