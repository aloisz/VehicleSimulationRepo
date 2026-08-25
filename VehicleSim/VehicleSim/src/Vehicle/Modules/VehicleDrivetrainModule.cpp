#include "VehicleDrivetrainModule.h"

#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"

#include <cmath>
#include <algorithm>

using namespace MathUtils;

namespace Vehicle 
{
    bool VehicleDrivetrainModule::Initialize(VehicleContext& context)
    {
        if (!context.IsValid())
        {
            Log::Error("VehicleDrivetrainModule: invalid context", Log::Category::Vehicle);
            return false;
        }

        const VehicleConfig& cfg = *context.Config;

        _drivenWheels.clear();
        _drivenFront.clear();
        _drivenRear.clear();
        _maxAxle = 0;

        for (const WheelConfig& wheel : cfg.Wheels) 
        {
            _maxAxle = std::max(_maxAxle, wheel.Axle);
        }

        for (size_t i = 0; i < cfg.Wheels.size(); ++i)
        {
            if (!cfg.Wheels[i].IsDriven) 
            {
                continue;
            }

            _drivenWheels.push_back(i);

            if (cfg.Wheels[i].Axle == 0) 
            {
                _drivenFront.push_back(i);
            }
            else _drivenRear.push_back(i);
        }

        if (_drivenWheels.empty())
        {
            Log::Error("VehicleDrivetrainModule: no driven wheels", Log::Category::Vehicle);
            return false;
        }

        context.Runtime->CurrentGear = 1;
        context.Runtime->PendingGear = 1;
        context.Runtime->GearRatio = GetTotalRatio(cfg.Gearbox, 1);

        Log::Info(std::string("Drivetrain: ") + ToString(cfg.Differential.Type) + ", " +
            std::to_string(_drivenWheels.size()) + " driven wheels, " +
            std::to_string(cfg.Gearbox.Ratios.size()) + " forward gears",
            Log::Category::Vehicle);

        _initialized = true;
        return true;
    }

    void VehicleDrivetrainModule::Update(float dt, VehicleContext& context)
    {
    }

    void Vehicle::VehicleDrivetrainModule::Dispose()
    {
        _drivenWheels.clear();
        _drivenFront.clear();
        _drivenRear.clear();
        _initialized = false;
    }

    void VehicleDrivetrainModule::UpdateGearbox(float dt, VehicleContext& context)
    {
    }

    void VehicleDrivetrainModule::UpdateClutch(float dt, VehicleContext& context)
    {
    }

    void VehicleDrivetrainModule::DistributeTorque(VehicleContext& context)
    {
    }
    void VehicleDrivetrainModule::RequestGear(int gear, VehicleContext& context)
    {
    }
    float VehicleDrivetrainModule::GetTotalRatio(const GearboxConfig& gearbox, int gear)
    {
        return 0.0f;
    }
}