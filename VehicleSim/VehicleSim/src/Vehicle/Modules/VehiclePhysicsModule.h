#pragma once

#include "IVehicleModule.h"
#include <vector>

namespace Vehicle
{
    
    /// <summary>
    /// Handles The bridge between the tire model and the rigid body
    /// 
    /// This module runs in two phases per fixed step, because some of its jobs
    /// have to happen before the wheel module and some after
    /// </summary>
    class VehiclePhysicsModule : public IVehicleModule
    {
    public:
        bool Initialize(VehicleContext& context) override;

        /// Phase 1 => Call before the wheel module
        void UpdateControls(float dt, VehicleContext& context);

        /// Phase 2 => Call after the wheel module
        void Update(float dt, VehicleContext& context) override;

        void Dispose() override;

        const char* GetName() const override { return "VehiclePhysicsModule"; }

    private:
        void ApplySteering(float dt, VehicleContext& context);
        void ApplyBrakes(VehicleContext& context);
        void ApplyTractionControl(VehicleContext& context);

        void ApplyWheelForces(VehicleContext& context);
        void ApplyAntiRoll(VehicleContext& context);

        /// <summary>
        /// Wheel index pairs sharing an axle
        /// </summary>
        struct AxlePair
        {
            size_t Left = 0;
            size_t Right = 0;
            int Axle = 0;
            bool Valid = false;
        };

        std::vector<AxlePair> _axlePairs;
        std::vector<size_t> _steerableWheels;
        int _maxAxle = 0;

        /// Fraction of the way from the contact patch to the wheel centre at
        /// which tire forces are applied 0 = contact patch (load transfer), 1 = wheel centre (no roll moment)
        static constexpr float TIRE_FORCE_HEIGHT = 0.5f;
    };
}
