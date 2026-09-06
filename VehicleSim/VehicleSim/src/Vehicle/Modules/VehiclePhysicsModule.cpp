#include "VehiclePhysicsModule.h"

#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"

#include <cmath>
#include <algorithm>

using namespace MathUtils;


namespace Vehicle 
{
	bool VehiclePhysicsModule::Initialize(VehicleContext& context)
	{
        if (!context.IsValid())
        {
            Log::Error("VehiclePhysicsModule: invalid context", Log::Category::Vehicle);
            return false;
        }

        const VehicleConfig& cfg = *context.Config;

        _axlePairs.clear();
        _steerableWheels.clear();
        _maxAxle = 0;

        for (const WheelConfig& w : cfg.Wheels) 
        {
            _maxAxle = std::max(_maxAxle, w.Axle);
        }

        for (size_t i = 0; i < cfg.Wheels.size(); ++i) 
        {
            if (cfg.Wheels[i].IsSteerable)
            {
                _steerableWheels.push_back(i);

            }
        }

        // Pair up wheels by axle, using the sign of the local X offset to tell
        // left from right
        for (int axle = 0; axle <= _maxAxle; ++axle)
        {
            AxlePair pair;
            pair.Axle = axle;

            int found = 0;
            for (size_t i = 0; i < cfg.Wheels.size(); ++i)
            {
                if (cfg.Wheels[i].Axle != axle) continue;

                if (cfg.Wheels[i].LocalOffset.getX() < 0.0f) 
                {
                    pair.Left = i;
                    ++found; 
                }
                else 
                { 
                    pair.Right = i; 
                    ++found; 
                }
            }

            pair.Valid = (found == 2);
            if (!pair.Valid && found > 0)
            {
                Log::Warning("Axle " + std::to_string(axle) +
                    " does not have exactly one wheel per side; anti-roll disabled for it",
                    Log::Category::Vehicle);
            }

            _axlePairs.push_back(pair);
        }

        _initialized = true;
        return true;
	}

	void VehiclePhysicsModule::UpdateControls(float dt, VehicleContext& context)
	{
        if (!_initialized) return;

        ApplySteering(dt, context);
        ApplyBrakes(context);
        ApplyTractionControl(context);
	}

	void VehiclePhysicsModule::Update(float dt, VehicleContext& context)
	{
        if (!_initialized) return;

        ApplyWheelForces(context);
        ApplyAntiRoll(context);
	}

	void VehiclePhysicsModule::Dispose()
	{
        _axlePairs.clear();
        _steerableWheels.clear();
        _initialized = false;
	}

	void VehiclePhysicsModule::ApplySteering(float dt, VehicleContext& context)
	{
	}

	void VehiclePhysicsModule::ApplyBrakes(VehicleContext& context)
	{
	}

	void VehiclePhysicsModule::ApplyTractionControl(VehicleContext& context)
	{
        const BrakeConfig& bc = context.Config->Brakes;
        if (!bc.TractionControlEnabled) return;

        VehicleRuntime& rt = *context.Runtime;
        if (rt.Throttle < 0.05f) return;

        for (WheelRuntime& wr : rt.Wheels)
        {
            if (!wr.Grounded || std::fabs(wr.DriveTorque) <= 0.0f) continue;

            // Positive slip ratio under power: the wheel is spinning up.
            const float slipInDriveDirection =
                wr.DriveTorque >= 0.0f ? wr.SlipRatio : -wr.SlipRatio;

            if (slipInDriveDirection > bc.TcSlipThreshold)
            {
                const float excess = slipInDriveDirection - bc.TcSlipThreshold;
                const float cut = Clamp(1.0f - excess * 3.0f, 0.1f, 1.0f);
                wr.DriveTorque *= cut;
            }
        }
	}

	void VehiclePhysicsModule::ApplyWheelForces(VehicleContext& context)
	{
        VehicleRuntime& rt = *context.Runtime;
        const btVector3 com = context.GetCenterOfMass();

        for (WheelRuntime& wr : rt.Wheels)
        {
            if (!wr.Grounded) continue;

            // Suspension force acts at the anchor, along the suspension axis
            if (wr.SuspensionForce.length2() > MathUtils::EPSILON && IsFinite(wr.SuspensionForce))
                context.Body->applyForce(wr.SuspensionForce, wr.AnchorWorld - com);

            if (wr.TireForce.length2() > MathUtils::EPSILON && IsFinite(wr.TireForce))
            {
                const btVector3 application =
                    wr.HitPoint + wr.HitNormal * (wr.WheelCenter - wr.HitPoint).length()
                    * TIRE_FORCE_HEIGHT;
                context.Body->applyForce(wr.TireForce, application - com);
            }
        }
	}

	void VehiclePhysicsModule::ApplyAntiRoll(VehicleContext& context)
	{
	}

}

