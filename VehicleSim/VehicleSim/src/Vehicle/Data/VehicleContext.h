#pragma once

#include <btBulletDynamicsCommon.h>

namespace Vehicle
{
    struct VehicleContext
    {
        btRigidBody* Body = nullptr;
        btDiscreteDynamicsWorld* World = nullptr;

        bool IsValid() const
        {
            return Body && World;
        }
    };
}
