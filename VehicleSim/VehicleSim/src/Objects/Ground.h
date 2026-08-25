#pragma once

#include "../Core/Actor.h"
#include <vector>

/// <summary>
/// A large ground
/// </summary>
class Ground : public Actor
{
public:
    explicit Ground(btDiscreteDynamicsWorld* world);
    ~Ground() override;

    void OnDestroy() override;
    void Render() override;

private:
    struct Slab
    {
        btRigidBody* Body = nullptr;
        btCollisionShape* Shape = nullptr;
        btDefaultMotionState* MotionState = nullptr;
        btVector3 Size = btVector3(1, 1, 1);
        btVector3 Color = btVector3(0.4f, 0.42f, 0.45f);
    };

    void AddSlab(const btVector3& position,
        const btVector3& size,
        const btVector3& color,
        float pitchRadians = 0.0f,
        float friction = 1.0f);

    btDiscreteDynamicsWorld* _world = nullptr;
    std::vector<Slab> _slabs;
};
