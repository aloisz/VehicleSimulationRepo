#pragma once

#include "IVehicleModule.h"

namespace Vehicle
{
    /// <summary>
    /// Handles drag and downforce
    /// </summary>
    class VehicleAerodynamicsModule : public IVehicleModule
    {
    public:
        bool Initialize(VehicleContext& context) override;
        void Update(float dt, VehicleContext& context) override;
        void Dispose() override;

        const char* GetName() const override { return "VehicleAerodynamicsModule"; }

    private:
        void ApplyDrag(VehicleContext& context);
        void ApplyDowforce(VehicleContext& context);

        float _dynamicPressureFactor = 0.0f;
        float _lastSpeedSquared = 0.0f;
    };
}
