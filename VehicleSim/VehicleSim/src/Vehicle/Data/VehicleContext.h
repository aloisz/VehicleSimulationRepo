#pragma once

#include <btBulletDynamicsCommon.h>

#include "VehicleConfig.h"
#include "VehicleRuntime.h"

namespace Vehicle
{
    struct VehicleContext
    {
        VehicleConfig* Config = nullptr;
        VehicleRuntime* Runtime = nullptr;
        btRigidBody* Body = nullptr;
        btDiscreteDynamicsWorld* World = nullptr;

        bool IsValid() const
        {
            return Config && Runtime && Body && World;
        }

        // === Frame Helper ===
        btVector3 GetRight() const { return Body->getCenterOfMassTransform().getBasis().getColumn(0); }
        btVector3 GetUp() const { return Body->getCenterOfMassTransform().getBasis().getColumn(1); }
        btVector3 GetForward() const { return Body->getCenterOfMassTransform().getBasis().getColumn(2); }

        btVector3 GetCenterOfMass() const { return Body->getCenterOfMassPosition(); }

        float GetGravity() const { return -World->getGravity().getY(); }
    };
}
