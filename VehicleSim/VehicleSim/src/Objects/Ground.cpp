#include "Ground.h"

#include "../Core/Log.h"
#include "../Core/MathUtils.h"
#include "../Render/Primitives.h"

#include <GL/freeglut.h>

Ground::Ground(btDiscreteDynamicsWorld* world)
    : _world(world)
{
    SetName("Ground");

    if (!_world)
    {
        Log::Error("Ground: no physics world", Log::Category::Physics);
        return;
    }

    // Main slab
    AddSlab(btVector3(0.0f, -1.0f, 0.0f),
        btVector3(1600.0f, 2.0f, 1600.0f),
        btVector3(0.411f, 0.529f, 0.705f));

    // ramp to launch off
    AddSlab(btVector3(0.0f, 0.6f, 45.0f),
        btVector3(14.0f, 0.6f, 18.0f),
        btVector3(0.45f, 0.40f, 0.32f),
        -10.0f * MathUtils::DEG2RAD);
}

Ground::~Ground()
{
    OnDestroy();
}

void Ground::AddSlab(const btVector3& position,
    const btVector3& size,
    const btVector3& color,
    float pitchRadians,
    float friction)
{
    Slab slab;
    slab.Size = size;
    slab.Color = color;

    slab.Shape = new btBoxShape(btVector3(size.getX() * 0.5f,
        size.getY() * 0.5f,
        size.getZ() * 0.5f));

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(position);

    if (pitchRadians != 0.0f) 
    {
        transform.setRotation(btQuaternion(btVector3(1.0f, 0.0f, 0.0f), pitchRadians));
    }

    slab.MotionState = new btDefaultMotionState(transform);

    // static
    btRigidBody::btRigidBodyConstructionInfo info(
        0.0f, slab.MotionState, slab.Shape, btVector3(0.0f, 0.0f, 0.0f));

    slab.Body = new btRigidBody(info);
    slab.Body->setFriction(friction);
    slab.Body->setRestitution(0.05f);

    _world->addRigidBody(slab.Body);

    _slabs.push_back(slab);
}

void Ground::OnDestroy()
{
    for (Slab& slab : _slabs)
    {
        if (slab.Body && _world) 
        {
            _world->removeRigidBody(slab.Body);
        }

        delete slab.Body;
        slab.Body = nullptr;

        delete slab.MotionState; 
        slab.MotionState = nullptr;

        delete slab.Shape;
        slab.Shape = nullptr;
    }

    _slabs.clear();
}

void Ground::Render()
{
    for (const Slab& slab : _slabs)
    {
        if (!slab.Body) continue;

        btScalar matrix[16];
        slab.Body->getWorldTransform().getOpenGLMatrix(matrix);

        glPushMatrix();
        glMultMatrixf(matrix);

        glColor3f(slab.Color.getX(), slab.Color.getY(), slab.Color.getZ());
        Primitives::DrawBox(slab.Size.getX(), slab.Size.getY(), slab.Size.getZ());

        glPopMatrix();
    }

    // Grid overlay on top of the main slab
    Primitives::DrawGrid(200.0f, 5.0f);
}
