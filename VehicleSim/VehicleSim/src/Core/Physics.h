#pragma once
#include <btBulletDynamicsCommon.h>

class Physics
{
public:
    Physics();
    ~Physics();

    void init();
    void stepSimulation(float deltaTime);
    btDiscreteDynamicsWorld* getWorld() { return world; }

private:
    btBroadphaseInterface* broadphase = nullptr;
    btDefaultCollisionConfiguration* collisionConfig = nullptr;
    btCollisionDispatcher* dispatcher = nullptr;
    btSequentialImpulseConstraintSolver* solver = nullptr;
    btDiscreteDynamicsWorld* world = nullptr;
};
