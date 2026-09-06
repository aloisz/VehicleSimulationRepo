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
            Log::Error("VehicleDrivetrainModule: has no driven wheels", Log::Category::Vehicle);
            return false;
        }

        context.Runtime->CurrentGear = 1;
        context.Runtime->PendingGear = 1;
        context.Runtime->GearRatio = GetTotalRatio(cfg.Gearbox, 1);

        _maxGear = static_cast<int>(context.Config->Gearbox.Ratios.size());

        Log::Info(std::string("Drivetrain: ") + ToString(cfg.Differential.Type) + ", " +
            std::to_string(_drivenWheels.size()) + " driven wheels, " +
            std::to_string(cfg.Gearbox.Ratios.size()) + " forward gears",
            Log::Category::Vehicle);

        _initialized = true;
        return true;
    }

    void VehicleDrivetrainModule::Update(float dt, VehicleContext& context)
    {
        if (!_initialized)
        {
            return;
        }

        UpdateWheelFeedback(context);
        UpdateGearbox(dt, context);
        UpdateClutch(dt, context);
        DistributeTorque(context);
    }

    void VehicleDrivetrainModule::Dispose()
    {
        _drivenWheels.clear();
        _drivenFront.clear();
        _drivenRear.clear();
        _initialized = false;
    }

    void VehicleDrivetrainModule::UpdateWheelFeedback(VehicleContext& context)
    {
        VehicleRuntime& rt = *context.Runtime;

        float sum = 0.0f;
        for (size_t i : _drivenWheels) 
        {
            sum += rt.Wheels[i].AngularVelocity;
        }

        rt.DrivenWheelAngularVelocity = sum / static_cast<float>(_drivenWheels.size());
        rt.GearRatio = GetTotalRatio(context.Config->Gearbox, rt.CurrentGear);
    }

    void VehicleDrivetrainModule::UpdateGearbox(float dt, VehicleContext& context)
    {
        VehicleRuntime& runtime = *context.Runtime;
        const VehicleConfig& config = *context.Config;
        const GearboxConfig& gearbox = config.Gearbox;
        
        if (ShiftingInProgress(dt, runtime, gearbox)) 
        {
            return;
        }

        if (ManualShifting(dt, context, runtime))
        {
            return;
        }

        if (!runtime.AutoGearbox) 
        {
            return;
        }

        const float speed = runtime.Speed;
        const float absoluteSpeed = context.Body->getLinearVelocity().length();

        bool anyGrounded = false;
        for (const WheelRuntime& w : runtime.Wheels) 
        {
            anyGrounded |= w.Grounded;
        }

        const bool stationary = anyGrounded &&
            absoluteSpeed < REVERSE_ENGAGE_SPEED &&
            std::fabs(speed) < REVERSE_ENGAGE_SPEED;

        if (runtime.CurrentGear >= 0 
            && stationary 
            && runtime.BrakeInput > gearbox.AutoReverseBrakeThreshold
            && runtime.ThrottleInput < gearbox.AutoReverseThrottleThreshold)
        {
            RequestGear(-1, context);
            Log::Info("Down shifting", Log::Category::Vehicle);
            return;
        }

        if (runtime.CurrentGear < 0 
            && speed > -REVERSE_ENGAGE_SPEED 
            && runtime.ThrottleInput > gearbox.AutoReverseThrottleThreshold
            && runtime.BrakeInput < gearbox.AutoReverseBrakeThreshold)
        {
            RequestGear(1, context);
            Log::Info("Up shifting", Log::Category::Vehicle);
            return;
        }

        if (runtime.CurrentGear < 0) 
        {
            return;
        }

        // === Neutral ===
        if (runtime.CurrentGear == 0)
        {
            if (runtime.ThrottleInput > 0.05f) 
            {
                RequestGear(1, context);
                Log::Info("Neutral", Log::Category::Vehicle);
            }
            return;
        }

        // === Automatic up / down shifting ===
        if (runtime.CurrentGear < _maxGear && runtime.EngineRPM > gearbox.UpshiftRPM)
        {
            RequestGear(runtime.CurrentGear + 1, context);
            Log::Info("Automatic down shifting", Log::Category::Vehicle);
            return;
        }

        if (runtime.CurrentGear > 1 && runtime.EngineRPM < gearbox.DownshiftRPM)
        {
            const float lowerRatio = GetTotalRatio(gearbox, runtime.CurrentGear - 1);
            const float currentRatio = GetTotalRatio(gearbox, runtime.CurrentGear);
            if (currentRatio > MathUtils::EPSILON)
            {
                const float projectedRPM = runtime.EngineRPM * (lowerRatio / currentRatio);

                if (projectedRPM < gearbox.UpshiftRPM * 0.95f)
                {
                    RequestGear(runtime.CurrentGear - 1, context);
                    Log::Info("Automatic up shifting", Log::Category::Vehicle);
                }
            }
        }
    }

    bool VehicleDrivetrainModule::ShiftingInProgress(float dt, VehicleRuntime& runtime, const GearboxConfig& gearbox)
    {
        if (runtime.IsShifting)
        {
            runtime.ShiftTimer -= dt;
            if (runtime.ShiftTimer <= 0.0f)
            {
                runtime.CurrentGear = runtime.PendingGear;
                runtime.IsShifting = false;
                runtime.ShiftTimer = 0.0f;
                runtime.GearRatio = GetTotalRatio(gearbox, runtime.CurrentGear);
            }

            runtime.RequestShiftUp = false;
            runtime.RequestShiftDown = false;

            return true;
        }

        return false;
    }

    bool VehicleDrivetrainModule::ManualShifting(float dt, VehicleContext& context, VehicleRuntime& runtime)
    {
        if (runtime.RequestShiftUp) 
        {
            runtime.RequestShiftUp = false;
            RequestGear(std::min(runtime.CurrentGear + 1, _maxGear), context);
            return true;
        }

        if (runtime.RequestShiftDown)
        {
            runtime.RequestShiftDown = false;
            RequestGear(std::max(runtime.CurrentGear - 1, -1), context);
            return true;
        }

        return false;
    }

    void VehicleDrivetrainModule::UpdateClutch(float dt, VehicleContext& context)
    {
        VehicleRuntime& runtime = *context.Runtime;
        const VehicleConfig& config = *context.Config;
        const GearboxConfig& gearbox = config.Gearbox;
        const EngineConfig& engine = config.Engine;

        float target = 0.0f;

        if (!runtime.IsShifting && runtime.CurrentGear != 0)
        {
            const float ratio = std::fabs(GetTotalRatio(gearbox, runtime.CurrentGear));
            const float wheelSideRPM =
                std::fabs(runtime.DrivenWheelAngularVelocity * RADS2RPM * ratio);

            const float lowerBound = engine.IdleRPM * 0.6f;
            const float upperBound = std::max(gearbox.ClutchEngageRPM, lowerBound + 1.0f);

            const float rpmLock = Clamp01((wheelSideRPM - lowerBound) / (upperBound - lowerBound));

            target = Clamp01(std::max(rpmLock, runtime.Throttle * 0.6f));
        }

        runtime.ClutchLock = MoveTowards(runtime.ClutchLock, target, 6.0f * dt);
    }

    void VehicleDrivetrainModule::DistributeTorque(VehicleContext& context)
    {

    }

    void VehicleDrivetrainModule::RequestGear(int gear, VehicleContext& context)
    {
        VehicleRuntime& runtime = *context.Runtime;
        const GearboxConfig& gearbox = context.Config->Gearbox;

        gear = std::max(-1, std::min(gear, _maxGear));

        if (gear == runtime.CurrentGear || runtime.IsShifting) 
        {
            return;
        }

        runtime.PendingGear = gear;
        runtime.IsShifting = true;
        runtime.ShiftTimer = gearbox.ShiftTime;
        Log::Info(std::to_string(gear) + " Pending Gear", Log::Category::Vehicle);
    }

    float VehicleDrivetrainModule::GetTotalRatio(const GearboxConfig& gearbox, int gear)
    {
        if (gear == 0) 
        {
            return 0.0f;
        }

        if (gear < 0) 
        {
            return -gearbox.ReverseRatio * gearbox.FinalDrive;
        }

        const size_t index = static_cast<size_t>(gear - 1);
        if (index >= gearbox.Ratios.size()) 
        {
            return 0.0f;
        }

        return gearbox.Ratios[index] * gearbox.FinalDrive;
    }
}