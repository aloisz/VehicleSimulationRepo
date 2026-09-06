#pragma once

#include "IVehicleModule.h"

namespace Vehicle
{
    /// <summary>
    /// Handles engine RPM tracking and torque output.
    /// </summary>
    class VehicleEngineModule : public IVehicleModule
    {
    public:
        bool Initialize(VehicleContext& context) override;
        void Update(float dt, VehicleContext& context) override;
        void Dispose() override;

        const char* GetName() const override { return "VehicleEngineModule"; }

    private:
        void UpdateRPM(float dt, VehicleContext& context);
        void CalculateTorque(VehicleContext& context);

        /// <summary>
        /// Normalized torque curve
        /// </summary>
        static float TorqueCurve(const EngineConfig& engine, float rpm);

        bool _revLimiterActive = false;
    };
}
