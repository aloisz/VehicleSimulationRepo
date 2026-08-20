#include "VehiclePhysicsModule.h"

bool Vehicle::VehiclePhysicsModule::Initialize(VehicleContext& context)
{
	return false;
}

void Vehicle::VehiclePhysicsModule::UpdateControls(float dt, VehicleContext& context)
{
}

void Vehicle::VehiclePhysicsModule::Update(float dt, VehicleContext& context)
{
}

void Vehicle::VehiclePhysicsModule::Dispose()
{
}

void Vehicle::VehiclePhysicsModule::ApplySteering(float dt, VehicleContext& context)
{
}

void Vehicle::VehiclePhysicsModule::ApplyBrakes(VehicleContext& context)
{
}

void Vehicle::VehiclePhysicsModule::ApplyTractionControl(VehicleContext& context)
{
}

void Vehicle::VehiclePhysicsModule::ApplyWheelForces(VehicleContext& context)
{
}

void Vehicle::VehiclePhysicsModule::ApplyAntiRoll(VehicleContext& context)
{
}
