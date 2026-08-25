#pragma once

#include "IVehicleModule.h"

namespace Vehicle
{
    /// <summary>
    /// This module computes forces and writes them into WheelRuntime
    /// Handles Everything that happens at the wheel, per fixed step:
    ///   UpdateGroundDetection() - raycast from the suspension anchor
    ///   UpdateSuspension()      - spring / damper, produces the wheel load
    ///   UpdateSlip()            - contact frame, slip ratio and slip angle
    ///   UpdateTireForces()      - slip curve + friction ellipse
    ///   IntegrateWheelSpin()    - wheel angular velocity from drive/brake/tire torque
    /// </summary>
    class VehicleWheelModule : public IVehicleModule
    {
    public:
        bool Initialize(VehicleContext& context) override;
        void Update(float dt, VehicleContext& context) override;
        void Dispose() override;

        const char* GetName() const override { return "VehicleWheelModule"; }

    private:
        void UpdateGroundDetection(VehicleContext& context, size_t index);
        void UpdateSuspension(VehicleContext& context, size_t index);
        void UpdateSlip(VehicleContext& context, size_t index);
        void UpdateTireForces(VehicleContext& context, size_t index);
        void IntegrateWheelSpin(float dt, VehicleContext& context, size_t index);

        /// A surface steeper than this counts as a wall.
        static constexpr float MIN_GROUND_DOT = 0.15f;
    };
}
