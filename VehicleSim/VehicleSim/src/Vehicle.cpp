

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

    worldRef = world;
    worldRef->addRigidBody(rigidBody);
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



    std::vector<float> X = {-wheelOffsetX,wheelOffsetX,-wheelOffsetX,wheelOffsetX};
    std::vector<float> Z = {wheelOffsetZ,wheelOffsetZ,-wheelOffsetZ, -wheelOffsetZ};

    for (int i = 0; i < 4; i++)
    {
        // Wheel position in local space 
        btVector3 wheelLocalPos(X[i], wheelOffsetY, Z[i]);

        // Get chassis world transform
        btTransform chassisTransform;
        rigidBody->getMotionState()->getWorldTransform(chassisTransform);

        // Convert wheel position to world space
        btVector3 rayFrom = chassisTransform * wheelLocalPos;
        
        // Suspension direction
        btVector3 suspensionDir = btVector3(0, -1, 0);
        float suspensionLength = 2.0f;
        
        btVector3 rayTo = rayFrom + suspensionDir * suspensionLength;
        btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
        
        worldRef->rayTest(rayFrom, rayTo, rayCallback);
        DebugDrawRay::addRaycast(rayFrom, rayTo, rayCallback);
        
        if (rayCallback.hasHit())
        {
            btVector3 hitPoint = rayCallback.m_hitPointWorld;
            btVector3 hitNormal = rayCallback.m_hitNormalWorld;

            float hitDistance = (rayFrom - hitPoint).length();

            float restLength = 1.0f;
            float compression = restLength - hitDistance;

            float springStrength = 20000.0f;
            float damping = 3000.0f;

            float forceMagnitude = compression * springStrength;
            btVector3 force = hitNormal * forceMagnitude;

            rigidBody->applyForce(force, rayFrom - rigidBody->getCenterOfMassPosition());
            std::cout << "Suspension hit distance: " << hitDistance << std::endl;
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
    glPushMatrix();
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
    glPopMatrix();

    glPopMatrix();

    // Flush debugs
    DebugDrawRay::drawAndFlush();
}