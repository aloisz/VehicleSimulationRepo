#pragma once

#include <vector>
#include <memory>

#include "Physics.h"
#include "Actor.h"

class Application
{
public:
    static Application& GetInstance();

    void Init(float fixedDeltaTime = 1.0f / 120.0f);
    
    /// <summary>
    /// 'frameDeltaTime' is real elapsed wall clock time for this frame
    /// </summary>
    /// <param name="frameDeltaTime"></param>
    void Update(float frameDeltaTime);
    void Render();
    void Cleanup();

    /// <summary>
    /// Takes ownership of the actor
    /// </summary>
    /// <param name="actor"></param>
    void AddActor(Actor* actor);

    Physics& GetPhysics() { return _physics; }
    float GetFixedDeltaTime() const { return _fixedDeltaTime; }

    /// <summary>
    /// Fraction of a fixed step left in the accumulator, for render interpolation
    /// </summary>
    /// <returns></returns>
    float GetInterpolationAlpha() const { return _accumulator / _fixedDeltaTime; }

private:
    Application() = default;
    ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Physics _physics;
    std::vector<std::unique_ptr<Actor>> _actors;

    float _fixedDeltaTime = 1.0f / 120.0f;
    float _accumulator = 0.0f;
    bool _cleanedUp = false;

    static constexpr int MAX_STEPS_PER_FRAME = 8;
};
