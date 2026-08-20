#include "Application.h"
#include "Log.h"

#include <algorithm>

Application& Application::GetInstance()
{
    static Application instance;
    return instance;
}

void Application::Init(float fixedDeltaTime)
{
    _fixedDeltaTime = fixedDeltaTime > 0.0f ? fixedDeltaTime : (1.0f / 120.0f);
    _accumulator = 0.0f;
    _cleanedUp = false;

    _physics.Init();
}

void Application::AddActor(Actor* actor)
{
    if (!actor) return;

    _actors.emplace_back(actor);
    actor->Awake();
}

void Application::Update(float frameDeltaTime)
{
    if (_cleanedUp) return;

    frameDeltaTime = std::min(frameDeltaTime, 0.25f);

    for (auto& actor : _actors)
    {
        if (!actor->IsStarted())
        {
            actor->Start();
            actor->MarkStarted();
        }
    }

    // Variable step pass first so the fixed steps below act on fresh input
    for (auto& actor : _actors)
        actor->Tick(frameDeltaTime);

    _accumulator += frameDeltaTime;

    int steps = 0;
    while (_accumulator >= _fixedDeltaTime && steps < MAX_STEPS_PER_FRAME)
    {
        for (auto& actor : _actors)
            actor->FixedTick(_fixedDeltaTime);

        _physics.StepOnce(_fixedDeltaTime);

        _accumulator -= _fixedDeltaTime;
        ++steps;
    }

    // Dropped frames => throw away the backlog
    if (steps >= MAX_STEPS_PER_FRAME)
        _accumulator = 0.0f;

    for (auto& actor : _actors)
        actor->LateTick(frameDeltaTime);
}

void Application::Render()
{
    for (auto& actor : _actors)
        actor->Render();
}

void Application::Cleanup()
{
    if (_cleanedUp) return;
    _cleanedUp = true;

    // Actors unregister their bodies from the world in OnDestroy, so this has
    // to run before the world itself goes away.
    for (auto& actor : _actors)
        actor->OnDestroy();

    _actors.clear();

    _physics.Shutdown();

    Log::Info("Application cleaned up", Log::Category::General);
}
