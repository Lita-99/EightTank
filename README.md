# EightTank

This is my custom, data-oriented wheeled vehicle physics framework. This system functions as an ongoing exploration into game engine physics, as I learn how to write code, and create gameplay systems in Unreal.

https://github.com/user-attachments/assets/2b86566b-5938-46c5-8d03-6fb47cf3612d

This system relies on using structs to organize the code and maintain function scope. It currently has the following functionality:

- Raycast collision detection: Traces downawrds from sockets defined within the VehicleConfigData data asset
- Hooke's law suspension, with damping: Real time calculation of spring compression and velocity-driven damping forces.
- Anti-roll stabilization: Shifts forces to stabilize the vehicle from rolling.
- Data-driven: A VehiclePawn blueprint class can be initialised entirely from the VehicleConfig data asset.

---

https://github.com/user-attachments/assets/2fe42f92-921a-4212-87ef-046f623a284d

<br><br>

<img width="1007" height="1250" alt="bilde" src="https://github.com/user-attachments/assets/d3ae5e5e-7f1e-4dcb-9ea1-4b48fe80fee3" />

---

The VehicleMovementComponent acts as the orchestrator, calling functions within the struct, in the necessary order.

```cpp
for (FVehicleAxle& Axle : Vehicle.Axles)
{
    Axle.Left.UpdateWorldTransform(Vehicle.Transform);
    Axle.Right.UpdateWorldTransform(Vehicle.Transform);

    Axle.Left.PerformTrace(BodyInstance, GetWorld());
    Axle.Left.DrawDebugTrace(GetWorld());
    Axle.Right.PerformTrace(BodyInstance, GetWorld());
    Axle.Right.DrawDebugTrace(GetWorld());
    
    Axle.Left.Suspension.CalculateForces(DeltaTime);
    Axle.Right.Suspension.CalculateForces(DeltaTime);
}
```
<br>

While the functions themselves, being defined within the struct, remain readable.

```cpp
void FVehicleSuspension::CalculateForces(const float DeltaTime)
{
	Compression = FMath::Max(0.0f, TravelLength - Length); 
	const float RawVelocity = (Length - PreviousLength) / DeltaTime;
	CompressionVelocity = FMath::Lerp(
		CompressionVelocity, 
		RawVelocity, 
		0.2f);
	PreviousLength = Length;
	
	SpringForce = Compression * Stiffness;
	DampingForce = -(CompressionVelocity * Damping);
	
	AccumulatedForce = FMath::Max(
		SpringForce + DampingForce,
		0.f);
}
```

---

Todo list:

- A major reorganisation of the code is required. Currently VehicleAxle.cpp contains the majority of the relevant code.
- Implement proper friction calculations - the current implementation is only an approximation
- Implement proper engine torque input/output as a drive force
