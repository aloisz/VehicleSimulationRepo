#pragma once

#include <vector>
#include "Physics.h"
#include "GameObject.h"

class Application
{

public:
    static Application& getInstance();

    void init();
    void update(float deltaTime);
    void render();
    void cleanup();

    void addGameObject(GameObject* obj);
    Physics& getPhysics() { return physics; }

private:
    Application() {}
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Physics physics;
    std::vector<GameObject*> gameObjects;
};

