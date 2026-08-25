#pragma once

#include <btBulletDynamicsCommon.h>
#include <vector>

namespace Vehicle
{
    /// <summary>
    /// Per-wheel simulation state. Written by the modules
    /// read by the renderer and the HUD
    /// </summary>
    struct WheelRuntime
    {
        // === Ground detection ===
        bool Grounded = false;
        btVector3 HitPoint = btVector3(0, 0, 0);
        btVector3 HitNormal = btVector3(0, 1, 0);

        /// <summary>
        /// World position of the suspension anchor on the chassis
        /// </summary>
        btVector3 AnchorWorld = btVector3(0, 0, 0);
        
        /// <summary>
        /// World position of the wheel centre after suspension resolution
        /// </summary>
        btVector3 WheelCenter = btVector3(0, 0, 0);


        // === Suspension ===

        /// <summary>
        /// Anchor to wheel centre distance (m)
        /// </summary>
        float CurrentLength = 0.0f;

        /// <summary>
        /// Length - CurrentLength, 0 = fully extended
        /// </summary>
        float Compression = 0.0f;

        /// <summary>
        /// Normal force from the suspension (N)
        /// </summary>
        float WheelLoad = 0.0f;
        btVector3 SuspensionForce = btVector3(0, 0, 0);


        // === Contact frame ===

        /// <summary>
        /// Steered, projected onto contact plane
        /// </summary>
        btVector3 Forward = btVector3(0, 0, 1);
        btVector3 Right = btVector3(1, 0, 0);


        // === Slip ===

        /// <summary>
        /// Contact patch velocity along Forward (m/s)
        /// </summary>
        float LongVelocity = 0.0f;

        /// <summary>
        /// Contact patch velocity along Right (m/s)
        /// </summary>
        float LatVelocity = 0.0f;
        float SlipRatio = 0.0f;
        
        /// <summary>
        /// In radians
        /// </summary>
        float SlipAngle = 0.0f;


        // === Tire forces ===
        float LongForce = 0.0f;
        float LatForce = 0.0f;
        btVector3 TireForce = btVector3(0, 0, 0);


        // === Rotation ===

        /// <summary>
        /// rad/s, positive = rolling forward
        /// </summary>
        float AngularVelocity = 0.0f; 

        /// <summary>
        /// Accumulated for rendering (radians)
        /// </summary>
        float SpinAngle = 0.0f;

        /// <summary>
        /// Radians, positive = right
        /// </summary>
        float SteerAngle = 0.0f;


        // === Torques ===
        float DriveTorque = 0.0f;
        float BrakeTorque = 0.0f;
    };

  
    /// <summary>
    /// Whole vehicle simulation state
    /// </summary>
    struct VehicleRuntime
    {
        // === Motion ===

        /// <summary>
        /// Signed forward speed (m/s)
        /// </summary>
        float Speed = 0.0f;

        /// <summary>
        /// Speed in km/h
        /// </summary>
        float SpeedKmh = 0.0f;       

        // === Powertrain ===
        float EngineRPM = 850.0f;
        float EngineTorque = 0.0f;

        /// <summary>
        /// -1 reverse, 0 neutral, 1..N forward
        /// </summary>
        int CurrentGear = 1;

        /// <summary>
        /// Current gear ratio * final drive
        /// </summary>
        float GearRatio = 0.0f;

        /// <summary>
        /// 0 = fully slipping, 1 = fully locked
        /// </summary>
        float ClutchLock = 1.0f;
        bool IsShifting = false;
        float ShiftTimer = 0.0f;
        int PendingGear = 1;

        /// <summary>
        /// Mean of the driven wheels (rad/s)
        /// </summary>
        float DrivenWheelAngularVelocity = 0.0f;

        // === Driver commands, raw ===
        float ThrottleInput = 0.0f;
        float BrakeInput = 0.0f;

        // === Driver commands, effective ===
        float Throttle = 0.0f;
        float Brake = 0.0f;

        /// <summary>
        /// -1 left  +1 right
        /// </summary>
        float Steering = 0.0f;
        float Handbrake = 0.0f;

        bool RequestShiftUp = false;
        bool RequestShiftDown = false;
        bool AutoGearbox = true;

        std::vector<WheelRuntime> Wheels;

        void Resize(size_t wheelCount) { Wheels.assign(wheelCount, WheelRuntime{}); }
    };
}
