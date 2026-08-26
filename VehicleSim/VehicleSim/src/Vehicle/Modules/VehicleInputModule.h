#pragma once

#include "IVehicleModule.h"

namespace Vehicle 
{
	/// <summary>
	/// Handles keyboard input
	/// </summary>
	class VehicleInputModule : public IVehicleModule
	{
	public:
		bool Initialize(VehicleContext& context) override;
		void Update(float dt, VehicleContext& context) override;
		void Dispose() override;

		const char* GetName() const override { return "VehicleInputModule"; }

	private:
		void UpdateInput(float dt, VehicleContext& context);

		float _throttle = 0.0f;
		float _brake = 0.0f;
		float _steer = 0.0f;
	};
}