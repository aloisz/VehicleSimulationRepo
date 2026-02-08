#pragma once

#include "GameObject.h"

class Ground : public GameObject
{
public:
    Ground(btDiscreteDynamicsWorld* world);
    void render() override;

private:
    float size;
};

