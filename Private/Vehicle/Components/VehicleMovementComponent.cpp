#include "Vehicle/Components/VehicleMovementComponent.h"
#include "Vehicle/Simulation/VehicleTypes.h"

#include "Vehicle/Data/WheelConfigData.h"
#include "Vehicle/Data/VehicleConfigData.h"

UVehicleMovementComponent::UVehicleMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UVehicleMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

bool UVehicleMovementComponent::InitialiseVehicle(
	USkeletalMeshComponent* InComponent,
	UVehicleConfigData* InConfigData)
{
	if (!IsValid(InConfigData) || !IsValid(InComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: InitialiseVehicle failed - Invalid arguments!"), *GetName());
		return false;
	}
	
	// Temporarily disable sim
	SetComponentTickEnabled(false);
	InComponent->SetSimulatePhysics(false);

	// Set Skeletal mesh
	if (!IsValid(InConfigData->SkeletalMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: InitialiseVehicle failed - Skeletal Mesh Invalid!"), *GetName());
		return false;
	}
	InComponent->SetSkeletalMeshAsset(InConfigData->SkeletalMesh);

	// Override PhAT if one is specified
	if (InConfigData->PhysicsAssetOverride)
	{
		
		InComponent->SetPhysicsAsset(InConfigData->PhysicsAssetOverride);
		UE_LOG(LogTemp, Warning, TEXT("Physics asset: %s"), InComponent->GetPhysicsAsset() ? TEXT("Valid") : TEXT("Null"));
	}
	
	
	InComponent->UpdateComponentToWorld();
	InComponent->RefreshBoneTransforms();
	
	// Store the body instance of specified vehicle hull bone
	BodyInstance = InComponent->GetBodyInstance(InConfigData->ChassisBoneName);
	if (BodyInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: BodyInstance initialise failed for bone: %s!"), 
			*GetName(), *InConfigData->ChassisBoneName.ToString());
		return false;
	}
	
	
	// Initialise Vehicle simulation data
	Vehicle.Initialise(InComponent, InConfigData);
	Vehicle.Mass = BodyInstance->GetBodyMass();
	Vehicle.WheelMassShare = Vehicle.Mass / (Vehicle.Axles.Num() * 2.f);
	
	for (FAxleSetup& Setup : InConfigData->AxleSetups)
	{
		UStaticMeshComponent* LeftMesh = NewObject<UStaticMeshComponent>(GetOwner());
		LeftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LeftMesh->SetStaticMesh(Setup.LeftWheel->WheelMesh);
		LeftMesh->RegisterComponent();
		LeftMesh->AttachToComponent(InComponent, FAttachmentTransformRules::KeepWorldTransform);
		WheelMeshes.Add(LeftMesh);

		UStaticMeshComponent* RightMesh = NewObject<UStaticMeshComponent>(GetOwner());
		RightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightMesh->SetStaticMesh(Setup.RightWheel->WheelMesh);
		RightMesh->RegisterComponent();
		RightMesh->AttachToComponent(InComponent, FAttachmentTransformRules::KeepWorldTransform);
		WheelMeshes.Add(RightMesh);
	}

	// Begin sim
	InComponent->SetSimulatePhysics(true);
	SetComponentTickEnabled(true);

	UE_LOG(LogTemp, Warning, TEXT("[%s]: Initialise success!"), *GetName());
	return true;
}


void UVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	Vehicle.Throttle = ThrottleInput;
	Vehicle.Steer = SteerInput;
	Vehicle.Brake = BrakeInput;
	
	
	Vehicle.UpdateSteering();
	Vehicle.Transform = BodyInstance->GetUnrealWorldTransform();
	Vehicle.LongitudinalForce(DeltaTime, BodyInstance);
	int32 MeshIndex = 0;
	for (FVehicleAxle& Axle : Vehicle.Axles)
	{
		
		Axle.Left.UpdateWorldTransform(Vehicle.Transform);
		Axle.Right.UpdateWorldTransform(Vehicle.Transform);
	
		Axle.Left.PerformTrace(BodyInstance, GetWorld());
		Axle.Right.PerformTrace(BodyInstance, GetWorld());
		
		Axle.Left.Suspension.CalculateForces(DeltaTime);
		Axle.Right.Suspension.CalculateForces(DeltaTime);
	
		Axle.CalculateAntiRoll();
		
		if (Axle.Left.Wheel.bGrounded)
		{
			Axle.Left.ApplyForces(BodyInstance, DeltaTime, Vehicle);
		}
		if (Axle.Right.Wheel.bGrounded)
		{
			Axle.Right.ApplyForces(BodyInstance, DeltaTime, Vehicle);
		}
		
		Axle.Left.Wheel.Update(DeltaTime);
		Axle.Right.Wheel.Update(DeltaTime);
		
		for (int32 Side = 0; Side < 2; Side++)
		{
			FVehicleWheelAssembly& Assembly = (Side == 0) ? Axle.Left : Axle.Right;

			FVector LocalPosition = Assembly.LocalTransform.GetLocation();
			LocalPosition.Z -= Assembly.Suspension.Length;

			FQuat SteerQuat = FQuat(FVector::UpVector, Assembly.Wheel.SteerAngleRad);
			FQuat SpinQuat  = FQuat(FVector::RightVector, Assembly.Wheel.CumulativeRotation);

			FTransform WheelTransform;
			WheelTransform.SetLocation(LocalPosition);
			WheelTransform.SetRotation(SteerQuat * SpinQuat * Assembly.LocalTransform.GetRotation());

			WheelMeshes[MeshIndex++]->SetRelativeTransform(WheelTransform);
		}
	}
}

