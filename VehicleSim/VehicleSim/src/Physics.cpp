
#include "Physics.h"

Physics::Physics()
    : broadphase(nullptr)
    , collisionConfig(nullptr)
    , dispatcher(nullptr)
    , solver(nullptr)
    , world(nullptr)
{
}

Physics::~Physics()
{
    if (world) delete world;
    if (solver) delete solver;
    if (dispatcher) delete dispatcher;
    if (collisionConfig) delete collisionConfig;
    if (broadphase) delete broadphase;
}

void Physics::init()
{
    broadphase = new btDbvtBroadphase();
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    solver = new btSequentialImpulseConstraintSolver();

    world = new btDiscreteDynamicsWorld(
        dispatcher, broadphase, solver, collisionConfig
    );

    world->setGravity(btVector3(0, -9.81f, 0));
}

void Physics::stepSimulation(float deltaTime)
{
    if (world)
    {
        world->stepSimulation(deltaTime, 10);
    }
}
