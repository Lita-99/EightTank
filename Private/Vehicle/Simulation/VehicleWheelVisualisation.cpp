#include "DrawDebugHelpers.h"
#include "Vehicle/Components/VehicleMovementComponent.h"




	/*
void UVehicleMovementComponent::DrawDebugWheel(FWheel& Wheel, FAxle& Axle)
{
	
	// Trace line: green on hit, red on miss
	DrawDebugLine(GetWorld(),
	Wheel.Mount.WorldLocation + Wheel.Mount.WorldUpVector * Wheel.Config.Radius, 
	Wheel.Mount.WorldLocation + Wheel.Mount.WorldUpVector * Axle.Derived.TraceLength,
	Wheel.Trace.bHit ? FColor::Green : FColor::Red,
	false,
	-1.f,
	0,1.f);
	
	
	
	// Wheel Circle representation
	FMatrix WheelMatrix = ;
	DrawDebugCircle(
	GetWorld(),
	,
	Config.WheelRadius,
	10,
	FColor::Green,
	false, -1.f,
	SDPG_Foreground,
	2.f,
	WheelMatrix.GetUnitAxis(EAxis::X),
	WheelMatrix.GetUnitAxis(EAxis::Z),
	false
	);
	
	// Neutral length marker
	DrawDebugPoint(GetWorld(),
	State.Mount.Location + State.Mount.UpVector * -Config.SuspensionNeutralLength,
	10.f,
	FColor::Blue,
	false,
	-1.f,
	1);
	
	// Trace Hit Point
	DrawDebugPoint(GetWorld(),
	State.Trace.HitLocation, 
	10.f, FColor::Red, 
	false, 
	-1.f, 
	1);
}*/