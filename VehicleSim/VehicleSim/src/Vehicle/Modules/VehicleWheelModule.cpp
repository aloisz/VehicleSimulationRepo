#include "VehicleWheelModule.h"

bool Vehicle::VehicleWheelModule::Initialize(VehicleContext& context)
{
	return false;
}

void Vehicle::VehicleWheelModule::Update(float dt, VehicleContext& context)
{
}

void Vehicle::VehicleWheelModule::Dispose()
{
}

void Vehicle::VehicleWheelModule::UpdateGroundDetection(VehicleContext& context, size_t index)
{
}

void Vehicle::VehicleWheelModule::UpdateSuspension(VehicleContext& context, size_t index)
{
}

void Vehicle::VehicleWheelModule::UpdateSlip(VehicleContext& context, size_t index)
{
}

void Vehicle::VehicleWheelModule::UpdateTireForces(VehicleContext& context, size_t index)
{
}

void Vehicle::VehicleWheelModule::IntegrateWheelSpin(float dt, VehicleContext& context, size_t index)
{
}
