#include "VehicleCore.h"

#include "../Core/Log.h"
#include "../Core/MathUtils.h"
#include "../Core/Input.h"
#include "../Render/Primitives.h"

#include <GL/freeglut.h>
#include <cmath>


Vehicle::VehicleCore::VehicleCore(
    btDiscreteDynamicsWorld* world,
    const VehicleConfig& config,
    const btVector3& spawnPosition,
    float spawnYaw)
    : _config(config)
    , _world(world)
{
    SetName(_config.Name);

    _spawnTransform.setIdentity();
    _spawnTransform.setOrigin(spawnPosition);
    _spawnTransform.setRotation(btQuaternion(btVector3(0.0f, 1.0f, 0.0f), spawnYaw));
}

Vehicle::VehicleCore::~VehicleCore()
{
    OnDestroy();
}

#pragma region LifeCycle
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
#pragma endregion LifeCycle

#pragma region Rigidbody
bool Vehicle::VehicleCore::CreateRigidBody()
{
    return false;
}

void Vehicle::VehicleCore::DestroyRigidBody()
{
    if (_rigidBody && _world)
        _world->removeRigidBody(_rigidBody);

    delete _rigidBody;
    _rigidBody = nullptr;

    delete _motionState;
    _motionState = nullptr;

    delete _compoundShape;
    _compoundShape = nullptr;

    delete _boxShape;
    _boxShape = nullptr;
}

void Vehicle::VehicleCore::Reset()
{
}
#pragma endregion Rigidbody

#pragma region Tick
void Vehicle::VehicleCore::Tick(float dt)
{
}

void Vehicle::VehicleCore::FixedTick(float fdt)
{
}

void Vehicle::VehicleCore::LateTick(float dt)
{
}

void Vehicle::VehicleCore::UpdateRuntimeMotion()
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
#pragma endregion Tick

#pragma region Render
void Vehicle::VehicleCore::Render()
{
    if (!_operational) return;

    RenderChassis();
    RenderWheels();
}

void Vehicle::VehicleCore::RenderChassis() const
{
    btScalar matrix[16];
    _rigidBody->getCenterOfMassTransform().getOpenGLMatrix(matrix);

    glPushMatrix();
    glMultMatrixf(matrix);

    glTranslatef(-_config.CenterOfMassOffset.getX(),
        -_config.CenterOfMassOffset.getY(),
        -_config.CenterOfMassOffset.getZ());

    glColor3f(0.80f, 0.22f, 0.20f);
    Primitives::DrawBox(_config.BodySize.getX(),
        _config.BodySize.getY(),
        _config.BodySize.getZ());

    // A small nose marker to mark the car front
    glPushMatrix();
    glTranslatef(0.0f, _config.BodySize.getY() * 0.35f, _config.BodySize.getZ() * 0.45f);
    glColor3f(0.95f, 0.9f, 0.6f);
    Primitives::DrawBox(_config.BodySize.getX() * 0.6f, 0.12f, 0.12f);
    glPopMatrix();

    glPopMatrix();
}

void Vehicle::VehicleCore::RenderWheels() const
{
}
#pragma endregion Render





