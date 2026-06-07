#pragma once

#include "Curves/CurveFloat.h"
#include "VehicleTypes.generated.h"


struct FVehicleBase;
class UWheelConfigData;
class UDrivetrainConfigData;
class UVehicleConfigData;

USTRUCT(BlueprintType)
struct FSurfaceModifier
{
	GENERATED_BODY()
	
	// Stiffness Scaling modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Surface Modifier")
	float StiffnessScale = 1.0f;
	
	// Damping Scaling modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Surface Modifier")
	float DampingScale = 1.0f;
	
	// Friction Scaling Modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Surface Modifier")
	float FrictionScale = 1.0f;
};

UENUM()
enum class EDifferentialType { Open, Locked, LimitedSlip };

USTRUCT()
struct FDifferentialConfig
{
	GENERATED_BODY()
	
	EDifferentialType Type;
	float LockFactor;
};

UENUM()
enum class ETransmissionMode { Drive, Neutral, Reverse, Braking };





USTRUCT(BlueprintType)
struct FAxleSetup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Config")
	FDifferentialConfig Differential;
	UPROPERTY(EditAnywhere, Category = "Config")
	bool bCanSteer = false;
	UPROPERTY(EditAnywhere, Category = "Config")
	bool bDriven = false;
	UPROPERTY(EditAnywhere, Category = "Config")
	bool bUseAckermann = true;
	UPROPERTY(EditAnywhere, Category = "Config")
	float MaxSteerAngle = 35.f;
	UPROPERTY(EditAnywhere, Category = "Config")
	float AntiRollStiffness = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Axle")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Suspension")
	float AxleLoadCapacity = 1000;
	
	UPROPERTY(EditDefaultsOnly, Category = "Suspension")
	float RestCompressionLengthRatio = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Suspension")
	float DampingRatio = 0.8f;

	UPROPERTY(EditDefaultsOnly, Category = "Suspension")
	float TravelLength = 80;

	
	UPROPERTY(EditDefaultsOnly, Category = "Axle")
	TObjectPtr<UWheelConfigData> LeftWheel = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Axle", meta = (GetOptions = "GetSocketOptions"))
	FName LeftWheelSocket;
	
	UPROPERTY(EditDefaultsOnly, Category = "Axle")
	TObjectPtr<UWheelConfigData> RightWheel = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Axle", meta = (GetOptions = "GetSocketOptions"))
	FName RightWheelSocket;
};

USTRUCT(BlueprintType)
struct FVehicleSuspension
{
	GENERATED_BODY()
	
	// Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Units = "kg"))
	float LoadCapacity = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float RestCompressionLengthRatio = 0.7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float DampingRatio = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Units = "cm"))
	float TravelLength = 0.0f;
	
	// Derived
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Derived", meta = (Units = "CM"))
	float TraceLength = 0.0f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Derived", meta = (Units = "N"))
	float Stiffness = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Derived", meta = (Units = "N"))
	float Damping = 0.0f;
	
	// State
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "cm"))
	float Length = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "cm"))
	float PreviousLength = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "cm"))
	float Compression = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "cm/s"))
	float CompressionVelocity = 0.0f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "N"))
	float SpringForce = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "N"))
	float DampingForce = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "N"))
	float AccumulatedForce = 0.0f;
	
	void Initialise(const FAxleSetup& InConfig);
	void CalculateForces(const float DeltaTime);
};

USTRUCT(BlueprintType)
struct FVehicleWheel
{
	GENERATED_BODY()
	
	// Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Radius = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float Mass = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float LateralGripFactor = 1.f;  
	
	
	
	
	// State
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVector Forward = FVector::XAxisVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVector Right   = FVector::YAxisVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVector Up      = FVector::ZAxisVector;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float AngularVelocity = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float CumulativeRotation = 0.0f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float SteerAngleRad = 0.0f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	bool bGrounded = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float NormalLoad = 0.0f;
	
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVector ContactPatchVelocity = FVector::ZeroVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVector ContactLocation = FVector::ZeroVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVector ContactNormal = FVector::ZeroVector;
	
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float LateralVelocity = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float LongitudinalVelocity = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float LongitudinalForce = 0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float LateralForce = 0.0f;

	void Initialise(const UWheelConfigData* InConfig);
	void Update(const float DeltaTime);
	
};

USTRUCT(BlueprintType)
struct FVehicleWheelAssembly
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FTransform LocalTransform = FTransform::Identity;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FTransform WorldTransform = FTransform::Identity;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVehicleSuspension Suspension;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FVehicleWheel Wheel;
	
	void UpdateWorldTransform(const FTransform& BodyTransform);
	void PerformTrace(FBodyInstance* BodyInstance, UWorld* World);
	void ApplyForces(
		FBodyInstance* BodyInstance, 
		const float DeltaTime,
		FVehicleBase& Vehicle);
};

USTRUCT(BlueprintType)
struct FVehicleAxle
{
	GENERATED_BODY()

	// Config
	UPROPERTY(EditAnywhere, Category = "Config")
	FDifferentialConfig Differential;
	UPROPERTY(EditAnywhere, Category = "Config")
	bool bCanSteer = false;
	UPROPERTY(EditAnywhere, Category = "Config")
	bool bDriven = false;
	UPROPERTY(EditAnywhere, Category = "Config")
	bool bUseAckermann = true;
	UPROPERTY(EditAnywhere, Category = "Config")
	float MaxSteerAngle = 35.f;
	UPROPERTY(EditAnywhere, Category = "Config")
	float AntiRollStiffness = 0.5f;
	
	// Derived
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Derived", meta = (Units = "N"))
	float AntiRollForcePerCm = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Derived", meta = (Units = "cm"))
	float LongitudinalPosition = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Derived", meta = (Units = "cm"))
	float LongitudinalPivotOffset = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Derived", meta = (Units = "cm"))
	float TrackWidth = 0.f;
	
	// State
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", meta = (Units = "cm"))
	float CompressionDelta = 0.0f;
	UPROPERTY(VisibleInstanceOnly, Category = "State")
	float AntiRollForce = 0.f;
	
	
	// Sub Structs
	UPROPERTY(VisibleInstanceOnly, Category = "WheelAssembly")
	FVehicleWheelAssembly Left;
	UPROPERTY(VisibleInstanceOnly, Category = "WheelAssembly")
	FVehicleWheelAssembly Right;

	void Initialise(FAxleSetup& Setup);
	void CalculateAntiRoll();
	void ResetSteering();
	
};

USTRUCT(BlueprintType)
struct FVehicleBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Config", meta = (Units = "cm/s"))
	float TopSpeed = 100.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Config", meta = (Units = "cm/s"))
	float MaxDriveForce = 1000.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Config")
	float DragCoefficient = 1.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	FTransform Transform;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float Mass = 5.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float WheelMassShare = 5.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float VehicleSpeed = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float NormalizedSpeed = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float DriveForce;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	float BleedForce;
	
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Axles")
	TArray<FVehicleAxle> Axles;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Axles")
	TArray<int32> NonSteerableAxleIndices;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Axles")
	TArray<int32> SteerableAxleIndices;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Axles")
	TArray<int32> DrivenAxleIndices;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input")
	float Throttle = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input")
	float Brake = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Input")
	float Steer = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ackermann")
	float AckermannPivotX = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ackermann")
	float AckermannRefOffset = 0.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ackermann")
	float AckermannRefMaxAngle = 0.f;
	
	
	void Initialise(
		const USkeletalMeshComponent* InComponent,
		UVehicleConfigData* InConfigData);
	
	
	void LongitudinalForce(const float DeltaTime, FBodyInstance* BodyInstance);
	void UpdateSteering();
	
};