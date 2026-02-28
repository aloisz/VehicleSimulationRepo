#pragma once
#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

struct DebugRayEntry
{
    glm::vec3 origin;
    glm::vec3 end;// actual endpoint (hit or maxDist)

    glm::vec3 color;// line color

    bool hasNormal;
    glm::vec3 normalStart; // where to draw normal from (= hit point)
    glm::vec3 normalEnd;
};


class DebugDrawRay
{
public:
    // Colors
    static constexpr float HIT_R  = 0.1f, HIT_G  = 1.0f, HIT_B  = 0.1f; // green
    static constexpr float MISS_R = 1.0f, MISS_G = 0.2f, MISS_B = 0.2f; // red
    static constexpr float NORM_R = 0.2f, NORM_G = 0.6f, NORM_B = 1.0f; // blue
    static constexpr float NORMAL_LENGTH = 0.3f;
    
    // Draws from origin to origin + direction * length
    static void addRay(const glm::vec3& origin, const glm::vec3& direction, float length = 100.0f,
                       const glm::vec3& color = glm::vec3(1.0f, 1.0f, 0.0f));

    // Add a ray from a Bullet raycast result
    static void addRaycast(const glm::vec3& origin, const glm::vec3& direction, float maxDist,
                           const btCollisionWorld::ClosestRayResultCallback& result);

    static void addRaycast(const btVector3& btFrom, const btVector3& btTo,
        const btCollisionWorld::ClosestRayResultCallback& result);

    // Call once per frame in your render loop
    static void drawAndFlush();

    // Manual clear if needed
    static void clear();

private:
    // storage
    static std::vector<DebugRayEntry>& rays()
    {
        static std::vector<DebugRayEntry> s_rays;
        return s_rays;
    }
};
