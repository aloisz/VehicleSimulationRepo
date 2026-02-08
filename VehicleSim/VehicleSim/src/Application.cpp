

#include "Application.h"

Application& Application::getInstance()
{
    static Application instance;
    return instance;
}

void Application::init()
{
    physics.init();
}

void Application::update(float deltaTime)
{
    physics.stepSimulation(deltaTime);

    for (auto* obj : gameObjects)
    {
        obj->update(deltaTime);
    }
}

void Application::render()
{
    for (auto* obj : gameObjects)
    {
        obj->render();
    }
}

void Application::addGameObject(GameObject* obj)
{
    gameObjects.push_back(obj);
}

void Application::cleanup()
{
    for (auto* obj : gameObjects)
    {
        delete obj;
    }
    gameObjects.clear();
}