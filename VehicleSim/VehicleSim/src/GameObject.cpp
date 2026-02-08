

#include "GameObject.h"


GameObject::GameObject()
    : rigidBody(nullptr)
    , collisionShape(nullptr)
    , motionState(nullptr)
{
}

GameObject::~GameObject()
{
    if (rigidBody) delete rigidBody;
    if (motionState) delete motionState;
    if (collisionShape) delete collisionShape;
}

glm::mat4 GameObject::getTransform() const
{
    if (!rigidBody) return glm::mat4(1.0f);

    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);

    glm::mat4 mat(1.0f);
    trans.getOpenGLMatrix(&mat[0][0]);

    return mat;
}