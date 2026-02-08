

#include "Vehicle.h"
#include <GL/freeglut.h>

Vehicle::Vehicle(btDiscreteDynamicsWorld* world, const btVector3& position)
    : bodySize(2.0f, 0.8f, 4.0f)
    , wheelSize(0.4f, 0.4f, 0.4f)
{
    // Create vehicle body (box shape for now)
    collisionShape = new btBoxShape(btVector3(
        bodySize.x / 2.0f,
        bodySize.y / 2.0f,
        bodySize.z / 2.0f
    ));

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(position);

    motionState = new btDefaultMotionState(startTransform);

    btScalar mass = 1000.0f;
    btVector3 inertia(0, 0, 0);
    collisionShape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(
        mass, motionState, collisionShape, inertia
    );

    rigidBody = new btRigidBody(rbInfo);
    rigidBody->setRestitution(0.1f);
    rigidBody->setFriction(0.9f);
    rigidBody->setActivationState(DISABLE_DEACTIVATION);

    world->addRigidBody(rigidBody);
}

void Vehicle::update(float deltaTime)
{
    // Vehicle-specific updates can go here
}

void Vehicle::applyForce(const btVector3& force)
{
    if (rigidBody)
        rigidBody->applyCentralForce(force);
}

void Vehicle::applyTorque(const btVector3& torque)
{
    if (rigidBody)
        rigidBody->applyTorque(torque);
}

void Vehicle::renderBox(float width, float height, float depth)
{
    float w = width / 2.0f;
    float h = height / 2.0f;
    float d = depth / 2.0f;

    glBegin(GL_QUADS);

    // Front face
    glNormal3f(0, 0, 1);
    glVertex3f(-w, -h, d); glVertex3f(w, -h, d);
    glVertex3f(w, h, d); glVertex3f(-w, h, d);

    // Back face
    glNormal3f(0, 0, -1);
    glVertex3f(-w, -h, -d); glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d); glVertex3f(w, -h, -d);

    // Top face
    glNormal3f(0, 1, 0);
    glVertex3f(-w, h, -d); glVertex3f(-w, h, d);
    glVertex3f(w, h, d); glVertex3f(w, h, -d);

    // Bottom face
    glNormal3f(0, -1, 0);
    glVertex3f(-w, -h, -d); glVertex3f(w, -h, -d);
    glVertex3f(w, -h, d); glVertex3f(-w, -h, d);

    // Right face
    glNormal3f(1, 0, 0);
    glVertex3f(w, -h, -d); glVertex3f(w, h, -d);
    glVertex3f(w, h, d); glVertex3f(w, -h, d);

    // Left face
    glNormal3f(-1, 0, 0);
    glVertex3f(-w, -h, -d); glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d); glVertex3f(-w, h, -d);

    glEnd();
}

void Vehicle::render()
{
    glPushMatrix();

    // Apply the physics transform
    glm::mat4 transform = getTransform();
    glMultMatrixf(&transform[0][0]);

    // Draw vehicle body
    glColor3f(0.8f, 0.2f, 0.2f);
    renderBox(bodySize.x, bodySize.y, bodySize.z);

    // Draw simple wheels (4 corners)
    glColor3f(0.1f, 0.1f, 0.1f);

    float wheelOffsetX = bodySize.x / 2.0f + wheelSize.x / 2.0f;
    float wheelOffsetY = -bodySize.y / 2.0f - wheelSize.y / 2.0f;
    float wheelOffsetZ = bodySize.z / 3.0f;

    // Front-left wheel
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();

    // Front-right wheel
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();

    // Back-left wheel
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();

    // Back-right wheel
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();

    glPopMatrix();
}