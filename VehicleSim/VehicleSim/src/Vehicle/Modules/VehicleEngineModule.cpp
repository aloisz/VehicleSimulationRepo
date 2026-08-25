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

        if (context.Config->Engine.Inertia <= 1e-3f)
        {
            Log::Error("VehicleEngineModule: engine inertia must be > 0", Log::Category::Vehicle);
            return false;
        }

        context.Runtime->EngineRPM = context.Config->Engine.IdleRPM;

        _initialized = true;
        return true;
	}

	void VehicleEngineModule::Update(float dt, VehicleContext& context)
	{
	}

	void VehicleEngineModule::Dispose()
	{
		_initialized = false;
	}
}


