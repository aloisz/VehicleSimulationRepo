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
        const btTransform& GetTransform() const { return Body->getCenterOfMassTransform(); }

        btVector3 GetRight() const { return Body->getCenterOfMassTransform().getBasis().getColumn(0); }
        btVector3 GetUp() const { return Body->getCenterOfMassTransform().getBasis().getColumn(1); }
        btVector3 GetForward() const { return Body->getCenterOfMassTransform().getBasis().getColumn(2); }

        btVector3 GetCenterOfMass() const { return Body->getCenterOfMassPosition(); }

        /// Converts a point authored in the design frame (chassis box centred on
        /// the origin) into the rigid body's local frame, which is centred on the
        /// centre of mass.
        btVector3 DesignToBody(const btVector3& designPoint) const
        {
            return designPoint - Config->CenterOfMassOffset;
        }

        /// Same conversion, but returns the point in world space.
        btVector3 DesignToWorld(const btVector3& designPoint) const
        {
            return Body->getCenterOfMassTransform() * DesignToBody(designPoint);
        }

        float GetGravity() const { return -World->getGravity().getY(); }

        /// Static load on one wheel with the vehicle at rest, used to normalise
        /// the tire load sensitivity curve.
        float GetNominalWheelLoad() const
        {
            const size_t count = Config->Wheels.empty() ? 1 : Config->Wheels.size();
            return Config->Mass * GetGravity() / static_cast<float>(count);
        }
    };
}
