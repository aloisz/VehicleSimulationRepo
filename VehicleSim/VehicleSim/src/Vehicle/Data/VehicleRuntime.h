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

        /// World position of the suspension anchor on the chassis
        btVector3 AnchorWorld = btVector3(0, 0, 0);
        /// World position of the wheel centre after suspension resolution
        btVector3 WheelCenter = btVector3(0, 0, 0);


        // === Suspension ===

        ///Anchor to wheel centre distance (m)
        float CurrentLength = 0.0f;

        ///Length - CurrentLength, 0 = fully extended
        float Compression = 0.0f;

        ///Normal force from the suspension (N)
        float WheelLoad = 0.0f;
        btVector3 SuspensionForce = btVector3(0, 0, 0);


        // === Contact frame ===

        ///Steered, projected onto contact plane
        btVector3 Forward = btVector3(0, 0, 1);
        btVector3 Right = btVector3(1, 0, 0);


        // === Slip ===

        ///Contact patch velocity along Forward (m/s)
        float LongVelocity = 0.0f;

        ///Contact patch velocity along Right (m/s)
        float LatVelocity = 0.0f;
        float SlipRatio = 0.0f;
        
        ///In radians
        float SlipAngle = 0.0f;


        // === Tire forces ===
        float LongForce = 0.0f;
        float LatForce = 0.0f;
        btVector3 TireForce = btVector3(0, 0, 0);


        // === Rotation ===

        ///rad/s, positive = rolling forward
        float AngularVelocity = 0.0f; 

        ///Accumulated for rendering (radians)
        float SpinAngle = 0.0f;

        ///Radians, positive = right
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

        ///Signed forward speed (m/s)
        float Speed = 0.0f;

        ///Speed in km/h
        float SpeedKmh = 0.0f;       

        // === Powertrain ===
        float EngineRPM = 850.0f;
        float EngineTorque = 0.0f;

        ///-1 reverse, 0 neutral, 1..N forward
        int CurrentGear = 1;

        ///Current gear ratio * final drive
        float GearRatio = 0.0f;

        /// 0 = fully slipping, 1 = fully locked
        float ClutchLock = 1.0f;
        bool IsShifting = false;
        float ShiftTimer = 0.0f;
        int PendingGear = 1;

        ///Mean of the driven wheels (rad/s)
        float DrivenWheelAngularVelocity = 0.0f;

        // === Driver commands, raw ===
        float ThrottleInput = 0.0f;
        float BrakeInput = 0.0f;

        // === Driver commands, effective ===
        float Throttle = 0.0f;
        float Brake = 0.0f;

        ///-1 left  +1 right
        float Steering = 0.0f;
        float Handbrake = 0.0f;

        bool RequestShiftUp = false;
        bool RequestShiftDown = false;
        bool AutoGearbox = true;

        std::vector<WheelRuntime> Wheels;

        void Resize(size_t wheelCount) { Wheels.assign(wheelCount, WheelRuntime{}); }
    };
}
