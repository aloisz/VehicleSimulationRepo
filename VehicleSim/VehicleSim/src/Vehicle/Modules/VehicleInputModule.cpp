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
		const InputConfig& inputCfg = context.Config->Input;

		const bool throttleHeld = input.IsKeyHeld(inputCfg.throttleInput) || input.IsSpecialHeld(GLUT_KEY_UP);
		const bool brakeHeld = input.IsKeyHeld(inputCfg.brakeInput) || input.IsSpecialHeld(GLUT_KEY_DOWN);
		const bool leftHeld = input.IsKeyHeld(inputCfg.steerLeftInput) || input.IsSpecialHeld(GLUT_KEY_LEFT);
		const bool rightHeld = input.IsKeyHeld(inputCfg.steerRightInput) || input.IsSpecialHeld(GLUT_KEY_RIGHT);

		_throttle = MoveTowards(_throttle, throttleHeld ? 1.0f : 0.0f,
			(throttleHeld ? THROTTLE_RATE : THROTTLE_RELEASE_RATE) * dt);

		_brake = MoveTowards(_brake, brakeHeld ? 1.0f : 0.0f,
			(brakeHeld ? BRAKE_RATE : BRAKE_RELEASE_RATE) * dt);

		const float maxAngle = steeringCfg.MaxSteerAngle > MathUtils::EPSILON ? steeringCfg.MaxSteerAngle : 1.0f;
		const float steerRate = steeringCfg.SteerRate / maxAngle;
		const float returnRate = steeringCfg.ReturnRate / maxAngle;

		float steerTarget = 0.0f;
		if (leftHeld)  steerTarget -= 1.0f;
		if (rightHeld) steerTarget += 1.0f;

		const bool returning = (steerTarget == 0.0f) ||
			(Sign(steerTarget) != Sign(_steer) && _steer != 0.0f);

		_steer = MoveTowards(_steer, steerTarget, (returning ? returnRate : steerRate) * dt);
		_steer = Clamp(_steer, -1.0f, 1.0f);

		runtime.ThrottleInput = _throttle;
		runtime.BrakeInput = _brake;
		runtime.Steering = _steer;

		if (input.IsKeyHeld(inputCfg.gearShiftUpInput))
		{
			runtime.RequestShiftUp = true;
		}
		if (input.IsKeyHeld(inputCfg.gearShiftDownInput))
		{
			runtime.RequestShiftDown = true;
		}

		if (input.WasKeyPressed(inputCfg.changeGearboxType))
		{
			runtime.AutoGearbox = !runtime.AutoGearbox;
			Log::Info(runtime.AutoGearbox ? "Gearbox: automatic" : "Gearbox: manual",
				Log::Category::Vehicle);
		}

		/*Log::Info("ThrottleInput: " + std::to_string(static_cast<float>(runtime.ThrottleInput)), Log::Category::Vehicle);
		Log::Info("BrakeInput: " + std::to_string(static_cast<float>(runtime.BrakeInput)), Log::Category::Vehicle);
		Log::Info("Steering: " + std::to_string(static_cast<float>(runtime.Steering)), Log::Category::Vehicle);
		Log::Info("\n", Log::Category::Vehicle);*/
	}
}


