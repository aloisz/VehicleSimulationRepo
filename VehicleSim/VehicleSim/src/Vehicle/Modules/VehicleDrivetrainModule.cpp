#include "VehicleDrivetrainModule.h"

bool Vehicle::VehicleDrivetrainModule::Initialize(VehicleContext& context)
{
	return false;
}

void Vehicle::VehicleDrivetrainModule::Update(float dt, VehicleContext& context)
{
}

void Vehicle::VehicleDrivetrainModule::Dispose()
{
}

void Vehicle::VehicleDrivetrainModule::UpdateGearbox(float dt, VehicleContext& context)
{
}

void Vehicle::VehicleDrivetrainModule::UpdateClutch(float dt, VehicleContext& context)
{
}

void Vehicle::VehicleDrivetrainModule::DistributeTorque(VehicleContext& context)
{
}
