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
	}

	void VehiclePhysicsModule::Update(float dt, VehicleContext& context)
	{
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
	}

	void VehiclePhysicsModule::ApplyWheelForces(VehicleContext& context)
	{
	}

	void VehiclePhysicsModule::ApplyAntiRoll(VehicleContext& context)
	{
	}

}

