

#include "Vehicle.h"
#include <iostream>
#include <GL/freeglut.h>
#include "DebugDrawRay.h"

Vehicle::Vehicle(btDiscreteDynamicsWorld* world, const btVector3& position)
    : bodySize(2.0f, 0.8f, 4.0f)
    , wheelSize(0.4f, 0.4f, 0.4f)
{
    // Create vehicle body
    collisionShape = new btBoxShape(btVector3(
        bodySize.x / 2.0f,
        bodySize.y / 2.0f,
        bodySize.z / 2.0f
    ));

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(position);

    motionState = new btDefaultMotionState(startTransform);

    vehicleMass = 1000.0f; // in kg
    btVector3 inertia(0, 0, 0);
    collisionShape->calculateLocalInertia(vehicleMass, inertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(
        vehicleMass, motionState, collisionShape, inertia
    );

    rigidBody = new btRigidBody(rbInfo);
    rigidBody->setRestitution(0.1f);
    rigidBody->setFriction(0.9f);
    rigidBody->setActivationState(DISABLE_DEACTIVATION);

    worldRef = world;

    // avoiding collision from suspension with vehicle mesh
    short vehicleGroup = btBroadphaseProxy::CharacterFilter;
    short vehicleMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter;


    worldRef->addRigidBody(rigidBody, vehicleGroup, vehicleMask);
}

Vehicle::~Vehicle()
{
    delete worldRef;
}

void Vehicle::update(float deltaTime)
{
    float wheelOffsetX = bodySize.x / 2.0f + wheelSize.x / 2.0f;
    float wheelOffsetY = -bodySize.y / 2.0f;
    float wheelOffsetZ = bodySize.z / 3.0f;

    std::vector<float> X = { -wheelOffsetX, wheelOffsetX, -wheelOffsetX, wheelOffsetX };
    std::vector<float> Z = { wheelOffsetZ,  wheelOffsetZ, -wheelOffsetZ, -wheelOffsetZ };

    const float gravity = worldRef->getGravity().getY();
    const float springStrength = 20000.0f;
    const float damperStrength = 3000.0f;
    const float restLength = 1.0f;
    const float suspLength = 2.0f;
    const float maxSuspForce = 50000.0f;
    const float weightPerWheel = (vehicleMass * gravity) / 4.0f;

    for (int i = 0; i < 4; i++)
    {
        btVector3 wheelLocalPos(X[i], wheelOffsetY, Z[i]);

        btTransform chassisTransform;
        rigidBody->getMotionState()->getWorldTransform(chassisTransform);

        btVector3 rayFrom = chassisTransform * wheelLocalPos;
        btVector3 suspensionDir(0, -1, 0);
        btVector3 rayTo = rayFrom + suspensionDir * suspLength;

        btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
        rayCallback.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
        rayCallback.m_collisionFilterMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter;

        worldRef->rayTest(rayFrom, rayTo, rayCallback);
        DebugDrawRay::addRaycast(rayFrom, rayTo, rayCallback);

        if (rayCallback.hasHit())
        {
            btVector3 hitPoint = rayCallback.m_hitPointWorld;
            btVector3 hitNormal = rayCallback.m_hitNormalWorld;

            float hitDistance = (rayFrom - hitPoint).length();

            // Spring force
            float suspensionCompression = suspLength - hitDistance;  
            float compressionFromRest = suspensionCompression - restLength;
            float springForce = springStrength * compressionFromRest + weightPerWheel;

            // Damping force
            float compressionVelocity = 0.0f;
            if (deltaTime > 0.0f)
            {
                compressionVelocity = (suspensionCompression - previousSuspensionCompressions[i]) / deltaTime;
            }
            float dampingForce = compressionVelocity * damperStrength;

            // Total force
            float totalSuspensionForce = springForce + dampingForce;
            totalSuspensionForce = glm::clamp(totalSuspensionForce, 0.0f, maxSuspForce);

            // Apply at wheel contact point
            btVector3 force = hitNormal * totalSuspensionForce;
            btVector3 forceOffset = rayFrom - rigidBody->getCenterOfMassPosition();
            rigidBody->applyForce(force, forceOffset);

            previousSuspensionCompressions[i] = suspensionCompression;
        }
        else
        {
            // Wheel not touching collisions
            previousSuspensionCompressions[i] = 0.0f;
        }
    }
    
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

btVector3 Vehicle::GetVehiclePosition()
{
    if(rigidBody)
        return rigidBody->getCenterOfMassPosition();
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

    // Draw simple wheels
    glColor3f(0.1f, 0.1f, 0.1f);

    float wheelOffsetX = bodySize.x / 2.0f + wheelSize.x / 2.0f;
    float wheelOffsetY = -bodySize.y / 2.0f - wheelSize.y / 2.0f;
    float wheelOffsetZ = bodySize.z / 3.0f;

    // FL
    /*glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();

    // FR
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();

    // RL
    glPushMatrix();
    glTranslatef(-wheelOffsetX, wheelOffsetY, -wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();

    // RR
    glPushMatrix();
    glTranslatef(wheelOffsetX, wheelOffsetY, -wheelOffsetZ);
    renderBox(wheelSize.x, wheelSize.y, wheelSize.z);
    glPopMatrix();*/

    glPopMatrix();

    // Flush debugs
    DebugDrawRay::drawAndFlush();
}