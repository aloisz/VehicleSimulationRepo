#pragma once

#include "GameObject.h"

class Vehicle : public GameObject
{

public:
    Vehicle(btDiscreteDynamicsWorld* world, const btVector3& position);
    void render() override;
    void update(float deltaTime) override;

    void applyForce(const btVector3& force);
    void applyTorque(const btVector3& torque);

private:
    void renderBox(float width, float height, float depth);

    glm::vec3 bodySize;
    glm::vec3 wheelSize;
};

