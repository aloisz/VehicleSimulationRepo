#include "VehicleEngineModule.h"

#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"

#include <cmath>

using namespace MathUtils;

namespace Vehicle
{
	bool VehicleEngineModule::Initialize(VehicleContext& context)
	{
        if (!context.IsValid())
        {
            Log::Error("VehicleEngineModule: invalid context", Log::Category::Vehicle);
            return false;
        }

        if (context.Config->Engine.Inertia <= MathUtils::EPSILON)
        {
            Log::Error("VehicleEngineModule: engine inertia must be > 0", Log::Category::Vehicle);
            return false;
        }

        context.Runtime->EngineRPM = context.Config->Engine.IdleRPM;

        _initialized = true;
        return true;
	}
	
    void VehicleEngineModule::Dispose()
	{
		_initialized = false;
	}

	void VehicleEngineModule::Update(float dt, VehicleContext& context)
	{
        if (!_initialized) 
        {
            return;
        }

        UpdateRPM(dt, context);
        CalculateTorque(context);

        const EngineConfig& engine = context.Config->Engine;
        VehicleRuntime& runtime = *context.Runtime;
        //Log::Info("RPM: " + std::to_string(runtime.EngineRPM), Log::Category::Vehicle);
	}

    void VehicleEngineModule::UpdateRPM(float dt, VehicleContext& context)
    {
        const EngineConfig& engine = context.Config->Engine;
        VehicleRuntime& runtime = *context.Runtime;

        float geardRPM = engine.IdleRPM;
        if (runtime.CurrentGear != 0 && std::fabs(runtime.GearRatio) > MathUtils::EPSILON)
        {
            const float wheelRPM = runtime.DrivenWheelAngularVelocity * RADS2RPM;
            geardRPM = std::fabs(wheelRPM * runtime.GearRatio);
        }

        const float netTorque = runtime.EngineTorque;
        const float angularAccel = netTorque / engine.Inertia;
        const float freeRPM = runtime.EngineRPM + angularAccel * dt * RADS2RPM;

        const float lock = Clamp01(runtime.ClutchLock);
        float rpm = Lerp(freeRPM, std::max(geardRPM, engine.IdleRPM), lock);

        rpm = Clamp(rpm, engine.IdleRPM, engine.MaxRPM);

        if (!std::isfinite(rpm)) 
        {
            rpm = engine.IdleRPM;
        }

        runtime.EngineRPM = rpm;
    }

    void VehicleEngineModule::CalculateTorque(VehicleContext& context)
    {
        const EngineConfig& engine = context.Config->Engine;
        VehicleRuntime& runtime = *context.Runtime;

        const float throttle = Clamp01(runtime.Throttle);
        float torque = engine.MaxTorque * TorqueCurve(engine, runtime.EngineRPM) * throttle;

        const float rpmFraction = Clamp01(runtime.EngineRPM / engine.MaxRPM);
        torque -= engine.EngineBrakeTorque * (1.0f - throttle) * rpmFraction;

        const bool atLimit = runtime.EngineRPM >= engine.MaxRPM * 0.995f;
        if (atLimit && torque > 0.0f) 
        {
            torque = 0.0f;
        }

        if (atLimit != _revLimiterActive) 
        {
            _revLimiterActive = atLimit;
        }

        runtime.EngineTorque = torque;
    }

    float VehicleEngineModule::TorqueCurve(const EngineConfig& engine, float rpm)
    {
        const float peak = engine.PeakTorqueRPM > 1.0f ? engine.PeakTorqueRPM : 1.0f;
        const float x = (rpm - peak) / peak;
        const float curve = 1.0f - 0.45f * x * x;

        return Clamp(curve, 0.2f, 1.0f);
    }
}


