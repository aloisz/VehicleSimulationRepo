#include "VehicleWheelModule.h"

#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"
#include "../../Render/DebugDraw.h"

#include <cmath>
#include <algorithm>

using namespace MathUtils;

namespace Vehicle 
{
	bool VehicleWheelModule::Initialize(VehicleContext& context)
	{
		if (!context.IsValid())
		{
			Log::Error("VehicleWheelModule: invalid context", Log::Category::Vehicle);
			return false;
		}

		if (context.Config->Wheels.empty())
		{
			Log::Error("VehicleWheelModule: config has no wheels", Log::Category::Vehicle);
			return false;
		}

		if (context.Runtime->Wheels.size() != context.Config->Wheels.size())
		{
			context.Runtime->Resize(context.Config->Wheels.size());
		}

		_initialized = true;
		return true;
	}

	void VehicleWheelModule::Update(float dt, VehicleContext& context)
	{
	}

	void VehicleWheelModule::Dispose()
	{
		_initialized = false;
	}

	void VehicleWheelModule::UpdateGroundDetection(VehicleContext& context, size_t index)
	{
	}

	void VehicleWheelModule::UpdateSuspension(VehicleContext& context, size_t index)
	{
	}

	void VehicleWheelModule::UpdateSlip(VehicleContext& context, size_t index)
	{
	}

	void VehicleWheelModule::UpdateTireForces(VehicleContext& context, size_t index)
	{
	}

	void VehicleWheelModule::IntegrateWheelSpin(float dt, VehicleContext& context, size_t index)
	{
	}
}

