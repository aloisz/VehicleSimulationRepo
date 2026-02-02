#include "Physics.h"
#include <btBulletDynamicsCommon.h>

btDiscreteDynamicsWorld* world = nullptr;

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

    // Ground plane
    btCollisionShape* groundShape =
        new btStaticPlaneShape(btVector3(0, 1, 0), 0);

    btDefaultMotionState* groundMotion =
        new btDefaultMotionState(btTransform::getIdentity());

    btRigidBody::btRigidBodyConstructionInfo groundInfo(
        0.0f, groundMotion, groundShape
    );

    world->addRigidBody(new btRigidBody(groundInfo));

    // Falling box
    btCollisionShape* boxShape =
        new btBoxShape(btVector3(1, 1, 1));

    btTransform boxTransform;
    boxTransform.setIdentity();
    boxTransform.setOrigin(btVector3(0, 10, 0));

    btDefaultMotionState* boxMotion =
        new btDefaultMotionState(boxTransform);

    btScalar mass = 1.0f;
    btVector3 inertia(0, 0, 0);
    boxShape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo boxInfo(
        mass, boxMotion, boxShape, inertia
    );

    world->addRigidBody(new btRigidBody(boxInfo));
}

void Physics::stepSimulation(float deltaTime)
{
    world->stepSimulation(deltaTime);
}
