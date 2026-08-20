#include "Physics.h"
#include "Log.h"

Physics::~Physics()
{
    Shutdown();
}

void Physics::Init(float gravity)
{
    if (_world)
    {
        Log::Warning("Physics::Init called twice", Log::Category::Physics);
        return;
    }

    _broadphase = new btDbvtBroadphase();
    _collisionConfig = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfig);
    _solver = new btSequentialImpulseConstraintSolver();

    _world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfig);
    _world->setGravity(btVector3(0.0f, gravity, 0.0f));

    _world->getSolverInfo().m_numIterations = EXTRA_SOLVER_ITERATION;

    Log::Info("Bullet world created", Log::Category::Physics);
}

void Physics::Shutdown()
{
    delete _world;
    _world = nullptr;

    delete _solver;
    _solver = nullptr;

    delete _dispatcher;
    _dispatcher = nullptr;

    delete _collisionConfig;
    _collisionConfig = nullptr;

    delete _broadphase;
    _broadphase = nullptr;
}

void Physics::StepOnce(float fixedDeltaTime)
{
    if (!_world) return;

    _world->stepSimulation(fixedDeltaTime, 1, fixedDeltaTime);
}
