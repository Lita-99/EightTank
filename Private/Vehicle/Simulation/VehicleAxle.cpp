#include "Vehicle/Simulation/VehicleTypes.h"
#include "Vehicle/Data/WheelConfigData.h"

void FVehicleAxle::Initialise(FAxleSetup& Setup)
{
	Differential = Setup.Differential;
	bCanSteer = Setup.bCanSteer;
	bDriven = Setup.bDriven;
	bUseAckermann = Setup.bUseAckermann;
	MaxSteerAngle = Setup.MaxSteerAngle;
	AntiRollStiffness = Setup.AntiRollStiffness;
	
	Left.Suspension.Initialise(Setup);
	Right.Suspension.Initialise(Setup);
	
	Left.Wheel.Initialise(Setup.LeftWheel);
	Right.Wheel.Initialise(Setup.RightWheel);
	
	Left.Suspension.TraceLength    = Left.Suspension.TravelLength 
									  + Left.Wheel.Radius;
	
	Right.Suspension.TraceLength    = Right.Suspension.TravelLength 
									  + Right.Wheel.Radius;
	
	
	AntiRollForcePerCm =
		(Left.Suspension.Stiffness + Right.Suspension.Stiffness) *
			AntiRollStiffness;
	
	
	const FVector LeftPos	= Left.LocalTransform.GetLocation();
	const FVector RightPos	= Right.LocalTransform.GetLocation();
		
	TrackWidth = FMath::Abs(LeftPos.Y - RightPos.Y);
	LongitudinalPosition = (LeftPos.X + RightPos.X) / 2.f;
		
}



void FVehicleWheelAssembly::ApplyForces(
	FBodyInstance* BodyInstance, 
	const float DeltaTime,
	FVehicleBase& Vehicle)
{
	

	// Suspension
	const FVector SuspForce = Wheel.Up * Suspension.AccumulatedForce;
	BodyInstance->AddForceAtPosition(SuspForce, Wheel.ContactLocation);
	
	// Lateral grip impulse
	const float DesiredLatChange = -Wheel.LateralVelocity * Wheel.LateralGripFactor;
	const FVector LatForce = Wheel.Right * (Vehicle.WheelMassShare * DesiredLatChange / DeltaTime);
	BodyInstance->AddForceAtPosition(LatForce, Wheel.ContactLocation);

	// Drive
	const FVector LongForce = Wheel.Forward * (Vehicle.DriveForce + Vehicle.BleedForce);
	BodyInstance->AddForceAtPosition(LongForce, Wheel.ContactLocation);
}

void FVehicleAxle::CalculateAntiRoll()
{
	CompressionDelta = Left.Suspension.Compression - Right.Suspension.Compression;
	AntiRollForce    = CompressionDelta * AntiRollForcePerCm;

	Left.Suspension.AccumulatedForce  += AntiRollForce;
	Left.Wheel.NormalLoad              = Left.Suspension.AccumulatedForce;

	Right.Suspension.AccumulatedForce -= AntiRollForce;
	Right.Wheel.NormalLoad             = Right.Suspension.AccumulatedForce;
}




void FVehicleAxle::ResetSteering()
{
	Left.Wheel.SteerAngleRad = 0.f;
	Right.Wheel.SteerAngleRad = 0.f;
}


void FVehicleWheelAssembly::UpdateWorldTransform(const FTransform& BodyTransform)
{
	// Resolve the mount point into world space.
	WorldTransform = LocalTransform * BodyTransform;


	// Extract world-space axes from the socket transform.
	const FQuat Rotation = WorldTransform.GetRotation();
	FVector Forward     = Rotation.GetAxisX();
	FVector Right       = Rotation.GetAxisY();
	const FVector Up    = Rotation.GetAxisZ();

	// Apply steering as rotation around the up axis.
	if (!FMath::IsNearlyZero(Wheel.SteerAngleRad))
	{
		Forward = Forward.RotateAngleAxisRad(Wheel.SteerAngleRad, Up);
		Right   = Right.RotateAngleAxisRad(Wheel.SteerAngleRad, Up);
	}

	Wheel.Forward = Forward;
	Wheel.Right   = Right;
	Wheel.Up      = Up;
}

void FVehicleWheelAssembly::PerformTrace(FBodyInstance* BodyInstance, UWorld* World)
{
	const FVector MountPoint = WorldTransform.GetLocation();
	const FVector Up       = WorldTransform.GetUnitAxis(EAxis::Z);
	
	const FVector TraceStart = MountPoint + (Up * Wheel.Radius);
	const FVector TraceEnd   = MountPoint - Up * (Suspension.TravelLength + Wheel.Radius);

	FHitResult Hit;
	const bool bHit = World->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_GameTraceChannel1);
	

	if (bHit)
	{
		Wheel.ContactLocation = Hit.ImpactPoint;
		Wheel.ContactNormal = Hit.ImpactNormal;
		Wheel.ContactPatchVelocity = BodyInstance->GetUnrealWorldVelocityAtPoint(Hit.ImpactPoint);
		Wheel.LongitudinalVelocity = FVector::DotProduct(
				Wheel.ContactPatchVelocity, Wheel.Forward);
		Wheel.LateralVelocity      = FVector::DotProduct(
			Wheel.ContactPatchVelocity, Wheel.Right);
		
		Suspension.Length = 
			FMath::Clamp(
				Hit.Distance - (Wheel.Radius * 2.f), 
				0.f,
				Suspension.TravelLength);
		
		if (!Wheel.bGrounded)
		{
			Suspension.PreviousLength = Suspension.Length;
			
			Wheel.bGrounded  = true;
		}
	}
	else
	{
		Wheel.bGrounded              = false;
		Wheel.ContactPatchVelocity   = FVector::ZeroVector;
		Wheel.ContactLocation        = FVector::ZeroVector;
		Wheel.ContactNormal          = FVector::ZeroVector;
		Suspension.Length            = Suspension.TravelLength; // Fully extended
	}
	
	
}
