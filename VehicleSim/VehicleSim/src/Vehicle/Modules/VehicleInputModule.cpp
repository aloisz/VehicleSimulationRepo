#include "VehicleInputModule.h"

#include "../../Core/Input.h"
#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"

#include <GL/freeglut.h>

using namespace MathUtils;

namespace Vehicle 
{
	bool VehicleInputModule::Initialize(VehicleContext& context)
	{
		if (!context.IsValid())
		{
			Log::Error("VehicleInputModule: invalid context", Log::Category::Vehicle);
			return false;
		}

		_initialized = true;
		return true;
	}

	void VehicleInputModule::Update(float dt, VehicleContext& context)
	{
	}

	void VehicleInputModule::Dispose()
	{
		_initialized = false;
	}
}


