#pragma once

namespace Primitives
{
    /// Axis-aligned box centred on the origin, given full extents.
    void DrawBox(float width, float height, float depth);

    /// Cylinder whose axis lies along local X, centred on the origin.
    /// This is the orientation a wheel wants: it rolls about X.
    void DrawCylinderX(float radius, float width, int segments = 16);

    /// Flat grid on the XZ plane, drawn as lines. Useful ground reference.
    void DrawGrid(float halfSize, float spacing);
}
