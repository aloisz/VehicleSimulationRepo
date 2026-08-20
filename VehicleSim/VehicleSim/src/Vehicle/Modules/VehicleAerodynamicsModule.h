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
    };
}
