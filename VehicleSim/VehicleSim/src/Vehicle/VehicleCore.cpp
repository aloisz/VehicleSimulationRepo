#include "VehicleCore.h"

Vehicle::VehicleCore::VehicleCore()
{

}

Vehicle::VehicleCore::~VehicleCore()
{
    OnDestroy();
}

void Vehicle::VehicleCore::Awake()
{
}

void Vehicle::VehicleCore::Start()
{
}

void Vehicle::VehicleCore::OnDestroy()
{
    if (!_operational && !_rigidBody) return;

    _input.Dispose();
    _engine.Dispose();
    _aerodynamics.Dispose();
    _wheel.Dispose();
    _drivetrain.Dispose();
    _physics.Dispose();

    DestroyRigidBody();

    _context = VehicleContext{};
    _operational = false;
}

void Vehicle::VehicleCore::Tick(float dt)
{
}

void Vehicle::VehicleCore::FixedTick(float fdt)
{
}

void Vehicle::VehicleCore::LateTick(float dt)
{
}

void Vehicle::VehicleCore::Render()
{
}

btVector3 Vehicle::VehicleCore::GetPosition() const
{
	return btVector3();
}

btVector3 Vehicle::VehicleCore::GetForward() const
{
	return btVector3();
}

void Vehicle::VehicleCore::Reset()
{
}

bool Vehicle::VehicleCore::InitializeModules()
{
    bool initialized = true;

    initialized &= _input.Initialize(_context);
    initialized &= _engine.Initialize(_context);
    initialized &= _aerodynamics.Initialize(_context);
    initialized &= _wheel.Initialize(_context);
    initialized &= _drivetrain.Initialize(_context);
    initialized &= _physics.Initialize(_context);

    return initialized;
}

bool Vehicle::VehicleCore::CreateRigidBody()
{
	return false;
}

void Vehicle::VehicleCore::DestroyRigidBody()
{
}

void Vehicle::VehicleCore::UpdateRuntimeMotion()
{
}

void Vehicle::VehicleCore::RenderChassis() const
{
}

void Vehicle::VehicleCore::RenderWheels() const
{
}
