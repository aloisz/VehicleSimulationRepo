#include "VehicleAerodynamicsModule.h"

#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"

#include <cmath>

namespace Vehicle 
{
	bool VehicleAerodynamicsModule::Initialize(VehicleContext& context)
	{
		if (!context.IsValid())
		{
			Log::Error("VehicleAerodynamicsModule: invalid context", Log::Category::Vehicle);
			return false;
		}

		const AerodynamicsConfig& aero = context.Config->Aero;
		_dynamicPressureFactor = 0.5f * aero.AirDensity * aero.FrontalArea;

		_initialized = true;
		return true;
	}

	void VehicleAerodynamicsModule::Update(float dt, VehicleContext& context)
	{

	}

	void VehicleAerodynamicsModule::Dispose()
	{
		_initialized = false;
	}

	void VehicleAerodynamicsModule::ApplyDrag(VehicleContext& context)
	{
	}

	void VehicleAerodynamicsModule::ApplyDowforce(VehicleContext& context)
	{
	}
}

