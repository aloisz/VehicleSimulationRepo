
#include "Ground.h"
#include <GL/freeglut.h>

Ground::Ground(btDiscreteDynamicsWorld* world)
    : size(50.0f)
{
    collisionShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, 0, 0));

    motionState = new btDefaultMotionState(groundTransform);

    btRigidBody::btRigidBodyConstructionInfo groundInfo(
        0.0f, motionState, collisionShape, btVector3(0, 0, 0)
    );

    rigidBody = new btRigidBody(groundInfo);
    rigidBody->setRestitution(0.3f);
    rigidBody->setFriction(0.8f);

    world->addRigidBody(rigidBody);
}

void Ground::render()
{
    glPushMatrix();

    // Draw a grid
    glColor3f(0.3f, 0.5f, 0.3f);
    glBegin(GL_LINES);

    for (int i = -size; i <= size; i += 2)
    {
        // Lines parallel to X axis
        glVertex3f(-size, 0, i);
        glVertex3f(size, 0, i);

        // Lines parallel to Z axis
        glVertex3f(i, 0, -size);
        glVertex3f(i, 0, size);
    }

    glEnd();
    glPopMatrix();
}