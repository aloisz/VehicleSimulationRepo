#pragma once

#include <btBulletDynamicsCommon.h>
#include <string>
#include <vector>
#include "../../Core/MathUtils.h"

namespace Vehicle
{
    enum class EDrivetrainType
    {
        FWD,
        RWD,
        AWD
    };

    const char* ToString(EDrivetrainType type);
    bool ParseDrivetrainType(const std::string& text, EDrivetrainType& out);

    struct SuspensionConfig
    {
        /// <summary>
        /// Max travel of the wheel below its anchor (m)
        /// </summary>
        float Length = 0.42f;

        /// <summary>
        /// N/m If <= 0, derived from mass and RestLength
        /// </summary>
        float SpringStrength = 0.0f;

        /// <summary>
        /// N per (m/s)
        /// </summary>
        float DamperStrength = 4200.0f;

        /// <summary>
        /// Clamp on the suspension force (N)
        /// </summary>
        float MaxForce = 60000.0f;

        /// <summary>
        /// Target suspension length at rest(m)
        /// </summary>
        float RestLength = 0.22f;
    };

    struct TireConfig
    {
        /// <summary>
        /// Slip ratio at peak longitudinal grip
        /// </summary>
        float PeakSlipRatio = 0.12f;

        /// <summary>
        /// Radians Slip angle at peak lateral grip
        /// </summary>
        float PeakSlipAngle = 0.14f;

        /// <summary>
        /// Longitudinal grip multiplier
        /// </summary>
        float LongGrip = 1.0f;

        /// <summary>
        /// Lateral grip multiplier
        /// </summary>
        float LatGrip = 1.05f;

        /// <summary>
        /// Base multiplier
        /// </summary>
        float FrictionCoefficient = 1.15f;

        /// <summary>
        /// How much mu drops as load rises above nominal
        /// </summary>
        float LoadSensitivity = 0.25f;

        /// <summary>
        /// Fraction of wheel load resisting rotation
        /// </summary>
        float RollingResistance = 0.015f;
    };

    struct WheelConfig
    {
        std::string Name = "Wheel";

        btVector3 LocalOffset = btVector3(0.0f, 0.0f, 0.0f);
        float Radius = 0.34f;
        float Width = 0.24f;

        /// <summary>
        /// Rotating mass, used for the wheels spin inertia
        /// </summary>
        float Mass = 22.0f;

        /// <summary>
        /// 0 = front, used for anti-roll pairing and brake bias
        /// </summary>
        int Axle = 0;         

        /// <summary>
        /// Overwritten at load time from the differential type
        /// </summary>
        bool IsDriven = false;    
        bool IsSteerable = false;

        SuspensionConfig Suspension;
        TireConfig Tire;

        /// <summary>
        /// Spin inertia of the wheel, modelled as a solid disc
        /// </summary>
        /// <returns></returns>
        float GetSpinInertia() const { return 0.5f * Mass * Radius * Radius; }
    };

    struct EngineConfig
    {
        float IdleRPM = 850.0f;
        float MaxRPM = 7000.0f;
        float PeakTorqueRPM = 4200.0f;

        /// <summary>
        /// Nm at the peak of the curve
        /// </summary>
        float MaxTorque = 380.0f;

        /// <summary>
        /// kg*m^2, govern free rev rate
        /// </summary>
        float Inertia = 0.35f;          

        /// <summary>
        /// Nm of drag at max RPM with a closed throttle
        /// </summary>
        float EngineBrakeTorque = 45.0f;  
    };

    struct GearboxConfig
    {
        std::vector<float> Ratios = { 3.4f, 2.1f, 1.4f, 1.05f, 0.85f, 0.7f };
        float FinalDrive = 3.7f;
        float ReverseRatio = 3.2f;
        float Efficiency = 0.92f;

        /// <summary>
        /// Seconds of torque cut during a shift
        /// </summary>
        float ShiftTime = 0.25f;
        float UpshiftRPM = 6300.0f;
        float DownshiftRPM = 2600.0f;

        float AutoReverseThrottleThreshold = 0.3f;
        float AutoReverseBrakeThreshold = 0.1f;

        /// <summary>
        /// RPM above which the clutch is fully locked
        /// </summary>
        float ClutchEngageRPM = 1400.0f; 
    };

    struct DifferentialConfig
    {
        EDrivetrainType Type = EDrivetrainType::RWD;

        /// <summary>
        /// AWD only: share of torque sent to axle 0
        /// </summary>
        float FrontBias = 0.5f;
    };

    struct AerodynamicsConfig
    {
        float AirDensity = 1.225f;
        float DragCoefficient = 0.34f;
        float FrontalArea = 2.2f;
        float DownforceCoefficient = 0.9f;

        /// <summary>
        /// Design frame
        /// </summary>
        btVector3 CenterOfPressureOffset = btVector3(0.0f, 0.2f, -0.4f); 
    };

    struct SteeringConfig
    {
        float MaxSteerAngle = 33.0f * MathUtils::RADIAN;

        /// <summary>
        /// Steer reduction at high speed
        /// </summary>
        float SpeedSensitivity = 0.6f;

        /// <summary>
        /// Radians per second of input travel
        /// </summary>
        float SteerRate = 180.0f * MathUtils::RADIAN;

        /// <summary>
        /// Self centering rate
        /// </summary>
        float ReturnRate = 300.0f * MathUtils::RADIAN;  
    };

    struct InputConfig 
    {
        char throttleInput = 'w'; 
        char brakeInput = 's';
        char steerLeftInput = 'a';
        char steerRightInput = 'd';

        char gearShiftUpInput = 'e';
        char gearShiftDownInput = 'q';

        char changeGearboxType = 'm';
    };

    struct BrakeConfig
    {
        /// <summary>
        /// Nm across the whole car
        /// </summary>
        float MaxBrakeTorque = 3000.0f;

        /// <summary>
        /// Share of brake torque on axle 0
        /// </summary>
        float FrontBias = 0.62f;

        /// <summary>
        /// Nm rear axle only.
        /// </summary>
        float HandbrakeTorque = 4000.0f;
        bool AbsEnabled = true;
        float AbsSlipThreshold = 0.14f;
        bool TractionControlEnabled = true;
        float TcSlipThreshold = 0.18f;
    };

    struct AntiRollConfig
    {
        /// <summary>
        /// N per meter of travel difference between sides
        /// </summary>
        float FrontStiffness = 6000.0f;
        float RearStiffness = 4500.0f;
    };

    /// <summary>
    /// Root config, populated from JSON at runtime <see cref"VehicleConfigLoader"/>
    /// </summary>
    struct VehicleConfig
    {
        std::string Name = "Vehicle";
        float Mass = 1400.0f;
        btVector3 BodySize = btVector3(1.9f, 1.1f, 4.4f); //Full extents (x, y, z)
        btVector3 CenterOfMassOffset = btVector3(0.0f, -0.35f, 0.0f); //Design frame

        EngineConfig Engine;
        GearboxConfig Gearbox;
        DifferentialConfig Differential;
        AerodynamicsConfig Aero;
        SteeringConfig Steering;
        InputConfig Input;
        BrakeConfig Brakes;
        AntiRollConfig AntiRoll;

        std::vector<WheelConfig> Wheels;

        bool Validate();

        static VehicleConfig MakeDefault();
    };
}
