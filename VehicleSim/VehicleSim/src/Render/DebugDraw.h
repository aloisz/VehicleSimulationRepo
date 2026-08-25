#pragma once

#include <btBulletDynamicsCommon.h>
#include <vector>
#include "../Core/Color.h"

/// <summary>
/// The simulation runs at a fixed step and the renderer at a variable one, so
/// rays are collected into a buffer during FixedTick and drawn once per frame.
/// Toggle with the 'F1' key in main.cpp.
/// </summary>
namespace DebugDraw
{
    struct Ray
    {
        btVector3 From;
        btVector3 To;
        bool Hit = false;
    };

    void SetEnabled(bool enabled);
    bool IsEnabled();

    void AddRay(const btVector3& from, const btVector3& to, bool hit);
    void AddLine(const btVector3& from, const btVector3& to, const btVector3& color);

    /// Draws everything collected so far and clears the buffers.
    void DrawAndFlush();

    /// Drops everything without drawing (used when debug draw is off).
    void Clear();
}
