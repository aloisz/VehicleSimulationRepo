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
		if (!_initialized) return;

		UpdateInput(dt, context);
	}

	void VehicleInputModule::Dispose()
	{
		_initialized = false;
	}

	void VehicleInputModule::UpdateInput(float dt, VehicleContext& context)
	{
		Input::Input& input = Input::Input::Get();

		VehicleRuntime& runtime = *context.Runtime;
		const SteeringConfig& steeringCfg = context.Config->Steering;

		// TODO : Need to store the input keys in VehicleConfig
		const bool throttleHeld = input.IsKeyHeld('w') || input.IsSpecialHeld(GLUT_KEY_UP);
		const bool brakeHeld = input.IsKeyHeld('s') || input.IsSpecialHeld(GLUT_KEY_DOWN);
		const bool leftHeld = input.IsKeyHeld('a') || input.IsSpecialHeld(GLUT_KEY_LEFT);
		const bool rightHeld = input.IsKeyHeld('d') || input.IsSpecialHeld(GLUT_KEY_RIGHT);

		Log::Info("throttleHeld: " + std::string(throttleHeld ? "true" : "false"), Log::Category::Vehicle);
		Log::Info("brakeHeld: " + std::string(brakeHeld ? "true" : "false"), Log::Category::Vehicle);
		Log::Info("leftHeld: " + std::string(leftHeld ? "true" : "false"), Log::Category::Vehicle);
		Log::Info("rightHeld: " + std::string(rightHeld ? "true" : "false"), Log::Category::Vehicle);
		Log::Info("\n", Log::Category::Vehicle);
	}
}


