#pragma once

#include "IVehicleModule.h"
#include <vector>

namespace Vehicle
{
    /// <summary>
    /// handles gearbox, clutch and torque split
    ///   UpdateGearbox()    - automatic or manual gear selection, shift timing
    ///   UpdateClutch()     - clutch lock as a function of RPM and shift state
    ///   DistributeTorque() - engine torque -> per-wheel drive torque
    /// </summary>
    class VehicleDrivetrainModule : public IVehicleModule
    {
    public:
        bool Initialize(VehicleContext& context) override;
        void Update(float dt, VehicleContext& context) override;
        void Dispose() override;

        const char* GetName() const override { return "VehicleDrivetrainModule"; }

    private:
        void UpdateGearbox(float dt, VehicleContext& context);
        void UpdateClutch(float dt, VehicleContext& context);
        void DistributeTorque(VehicleContext& context);
    };
}
