#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    virtual void update(float deltaTime) {}
    virtual void render() = 0;

    glm::mat4 getTransform() const;
    btRigidBody* getRigidBody() { return rigidBody; }

protected:
    btRigidBody* rigidBody;
    btCollisionShape* collisionShape;
    btDefaultMotionState* motionState;
};

