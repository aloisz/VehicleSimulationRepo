#pragma once
#include <btBulletDynamicsCommon.h>

class Physics
{
public:
    void init();
    void stepSimulation(float deltaTime);

private:
    btBroadphaseInterface* broadphase = nullptr;
    btDefaultCollisionConfiguration* collisionConfig = nullptr;
    btCollisionDispatcher* dispatcher = nullptr;
    btSequentialImpulseConstraintSolver* solver = nullptr;
    btDiscreteDynamicsWorld* world = nullptr;
};
