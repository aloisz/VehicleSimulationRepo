#pragma once

#include <btBulletDynamicsCommon.h>
#include <cmath>
#include <algorithm>

namespace MathUtils
{
    constexpr float PI = 3.14159265358979323846f;
    constexpr float RADIAN = 0.0174532925f;
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;
    constexpr float RPM2RADS = 2.0f * PI / 60.0f;
    constexpr float RADS2RPM = 60.0f / (2.0f * PI);

    constexpr float MSTOKMH = 3.6f;
    constexpr float KMHTOMS = 0.277778f;

    constexpr float EPSILON = 1e-6f;

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
    inline float MoveTowards(float current, float target, float maxDelta)
    {
        const float diff = target - current;
        if (std::fabs(diff) <= maxDelta) return target;
        return current + Sign(diff) * maxDelta;
    }

    inline float Pacejka(float Slip, float B, float C, float D, float E)
    {
        float bx = B * Slip;
        float term = bx - E * (bx - std::atan(bx));
        return D * std::sin(C * std::atan(term));
    }

    inline float SlipCurve(float slip, float peak)
    {
        if (peak <= EPSILON) return 0.0f;

        const float x = slip / peak;
        const float ax = std::fabs(x);

        if (ax <= 1.0f)
            return x;
        const float t = Clamp01((ax - 1.0f) / 2.0f);
        return Sign(x) * Lerp(1.0f, 0.75f, t);
    }

    inline bool IsFinite(const btVector3& v)
    {
        return std::isfinite(v.getX()) && std::isfinite(v.getY()) && std::isfinite(v.getZ());
    }
}
