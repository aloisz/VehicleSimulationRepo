#pragma once

#include <btBulletDynamicsCommon.h>
#include <cmath>
#include <algorithm>

namespace MathUtils
{
    constexpr float PI = 3.14159265358979323846f;
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;
    constexpr float RPM2RADS = 2.0f * PI / 60.0f;
    constexpr float RADS2RPM = 60.0f / (2.0f * PI);

    inline float Clamp(float value, float low, float high)
    {
        return value < low ? low : (value > high ? high : value);
    }

    inline float Clamp01(float value) { return Clamp(value, 0.0f, 1.0f); }

    inline float Lerp(float a, float b, float t) { return a + (b - a) * Clamp01(t); }

    inline float Sign(float value) { return value > 0.0f ? 1.0f : (value < 0.0f ? -1.0f : 0.0f); }

    /// <summary>
    /// Moves 'current' towards 'target' by at most 'maxDelta'
    /// </summary>
    /// <param name="current"></param>
    /// <param name="target"></param>
    /// <param name="maxDelta"></param>
    /// <returns></returns>
    inline float MoveTowards(float current, float target, float maxDelta)
    {
        const float diff = target - current;
        if (std::fabs(diff) <= maxDelta) return target;
        return current + Sign(diff) * maxDelta;
    }

    inline bool IsFinite(const btVector3& v)
    {
        return std::isfinite(v.getX()) && std::isfinite(v.getY()) && std::isfinite(v.getZ());
    }
}
