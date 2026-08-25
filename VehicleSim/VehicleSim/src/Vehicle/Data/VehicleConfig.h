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
        /// Max travel of the wheel below its anchor (m)
        float Length = 0.42f;

        /// N/m If <= 0, derived from mass and RestLength
        float SpringStrength = 0.0f;

        /// N per (m/s)
        float DamperStrength = 4200.0f;

        /// Clamp on the suspension force (N)
        float MaxForce = 60000.0f;

        /// Target suspension length at rest(m)
        float RestLength = 0.22f;
    };

    struct TireConfig
    {
        /// Slip ratio at peak longitudinal grip
        float PeakSlipRatio = 0.12f;

        /// Radians Slip angle at peak lateral grip
        float PeakSlipAngle = 0.14f;

        /// Longitudinal grip multiplier
        float LongGrip = 1.0f;

        /// Lateral grip multiplier
        float LatGrip = 1.05f;

        /// Base multiplier
        float FrictionCoefficient = 1.15f;

        /// How much mu drops as load rises above nominal
        float LoadSensitivity = 0.25f;

        /// Fraction of wheel load resisting rotation
        float RollingResistance = 0.015f;
    };

    struct WheelConfig
    {
        std::string Name = "Wheel";

        btVector3 LocalOffset = btVector3(0.0f, 0.0f, 0.0f);
        float Radius = 0.34f;
        float Width = 0.24f;

        /// Rotating mass, used for the wheels spin inertia
        float Mass = 22.0f;

        /// 0 = front, used for anti-roll pairing and brake bias
        int Axle = 0;         

        ///Overwritten at load time from the differential type
        bool IsDriven = false;    
        bool IsSteerable = false;

        SuspensionConfig Suspension;
        TireConfig Tire;

        /// Spin inertia of the wheel, modelled as a solid disc.
        float GetSpinInertia() const { return 0.5f * Mass * Radius * Radius; }
    };

    struct EngineConfig
    {
        float IdleRPM = 850.0f;
        float MaxRPM = 7000.0f;
        float PeakTorqueRPM = 4200.0f;

        /// Nm at the peak of the curve
        float MaxTorque = 380.0f;

        ///kg*m^2, govern free rev rate
        float Inertia = 0.35f;          

        /// Nm of drag at max RPM with a closed throttle
        float EngineBrakeTorque = 45.0f;  
    };

    struct GearboxConfig
    {
        std::vector<float> Ratios = { 3.4f, 2.1f, 1.4f, 1.05f, 0.85f, 0.7f };
        float FinalDrive = 3.7f;
        float ReverseRatio = 3.2f;
        float Efficiency = 0.92f;

        ///Seconds of torque cut during a shift
        float ShiftTime = 0.25f;
        float UpshiftRPM = 6300.0f;
        float DownshiftRPM = 2600.0f;

        ///RPM above which the clutch is fully locked
        float ClutchEngageRPM = 1400.0f; 
    };

    struct DifferentialConfig
    {
        EDrivetrainType Type = EDrivetrainType::RWD;

        ///AWD only: share of torque sent to axle 0
        float FrontBias = 0.5f;
    };

    struct AerodynamicsConfig
    {
        float AirDensity = 1.225f;
        float DragCoefficient = 0.34f;
        float FrontalArea = 2.2f;
        float DownforceCoefficient = 0.9f;

        /// Design frame
        btVector3 CenterOfPressureOffset = btVector3(0.0f, 0.2f, -0.4f); 
    };

    struct SteeringConfig
    {
        float MaxSteerAngle = 33.0f * MathUtils::RADIAN;

        ///Steer reduction at high speed
        float SpeedSensitivity = 0.6f;

        ///Radians per second of input travel
        float SteerRate = 180.0f * MathUtils::RADIAN;

        ///Self centering rate
        float ReturnRate = 300.0f * MathUtils::RADIAN;  
    };

    struct BrakeConfig
    {
        /// Nm across the whole car
        float MaxBrakeTorque = 3000.0f;

        ///Share of brake torque on axle 0
        float FrontBias = 0.62f;

        ///Nm rear axle only.
        float HandbrakeTorque = 4000.0f;
        bool AbsEnabled = true;
        float AbsSlipThreshold = 0.14f;
        bool TractionControlEnabled = true;
        float TcSlipThreshold = 0.18f;
    };

    struct AntiRollConfig
    {
        ///N per meter of travel difference between sides
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
        BrakeConfig Brakes;
        AntiRollConfig AntiRoll;

        std::vector<WheelConfig> Wheels;

        bool Validate();

        static VehicleConfig MakeDefault();
    };
}
