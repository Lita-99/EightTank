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


The wheel origin traces are defined by the sockets set within the Vehicle Config data asset. These are cached on initialisation, and updates every frame manually, by calling this:
```cpp
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
```

It's important to note that the Actor root, and the physics body root, are exactly the same. If the bone with the physics body is offset from the root bone of the skeletal mesh, the sockets will be offset in the actor, and thus trace from the wrong transform. I'm considering how to reapproach this setup, to make the system more stable, but keep it intuitive, while hopefully avoiding setting up components.

---

Todo list:

- A major reorganisation of the code is required. Currently VehicleAxle.cpp contains the majority of the relevant code.
- Implement proper friction calculations - the current implementation is only an approximation
- Implement proper engine torque input/output as a drive force

I also need to take a deep dive into the low level aspects of chaos physics, and understand how to make physics queries that don't require GetWorld(), to potentially allow for Async physics tick implementation, for deterministic simulation
