#pragma once

#include <btBulletDynamicsCommon.h>

/// <summary>
/// The Application drives one deterministic step at a time so that
/// forces pushed by FixedTick land on exactly one integration step
/// </summary>
class Physics
{
public:
    Physics() = default;
    ~Physics();

    Physics(const Physics&) = delete;
    Physics& operator=(const Physics&) = delete;

    void Init(float gravity = -9.81f);
    void Shutdown();

    /// <summary>
    /// Advances the world by exactly 'fixedDeltaTime' with no interpolation
    /// </summary>
    /// <param name="fixedDeltaTime"></param>
    void StepOnce(float fixedDeltaTime);

    btDiscreteDynamicsWorld* GetWorld() { return _world; }
    const btDiscreteDynamicsWorld* GetWorld() const { return _world; }

    bool IsInitialized() const { return _world != nullptr; }

private:
    btBroadphaseInterface* _broadphase = nullptr;
    btDefaultCollisionConfiguration* _collisionConfig = nullptr;
    btCollisionDispatcher* _dispatcher = nullptr;
    btSequentialImpulseConstraintSolver* _solver = nullptr;
    btDiscreteDynamicsWorld* _world = nullptr;

    static constexpr int EXTRA_SOLVER_ITERATION = 12;
};
