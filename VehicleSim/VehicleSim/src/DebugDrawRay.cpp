#include "DebugDrawRay.h"

void DebugDrawRay::addRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec3& color)
{
    DebugRayEntry e;
    e.origin = origin;
    e.end = origin + glm::normalize(direction) * length;
    e.color = color;
    e.hasNormal = false;
    rays().push_back(e);
}

void DebugDrawRay::addRaycast(const glm::vec3& origin, const glm::vec3& direction, float maxDist,
    const btCollisionWorld::ClosestRayResultCallback& result)
{
    DebugRayEntry e;
    e.origin = origin;

    glm::vec3 dir = glm::normalize(direction);

    if (result.hasHit())
    {
        // endpoint = hit point
        btVector3 hp = result.m_hitPointWorld;
        e.end = glm::vec3(hp.x(), hp.y(), hp.z());
        e.color = glm::vec3(HIT_R, HIT_G, HIT_B);

        // surface normal
        btVector3 n  = result.m_hitNormalWorld;
        glm::vec3 nv = glm::vec3(n.x(), n.y(), n.z());
        e.hasNormal = true;
        e.normalStart = e.end;
        e.normalEnd = e.end + nv * NORMAL_LENGTH;
    }
    else
    {
        e.end = origin + dir * maxDist;
        e.color = glm::vec3(MISS_R, MISS_G, MISS_B);
        e.hasNormal = false;
    }

    rays().push_back(e);
}

void DebugDrawRay::addRaycast(const btVector3& btFrom, const btVector3& btTo,
    const btCollisionWorld::ClosestRayResultCallback& result)
{
    DebugRayEntry e;
    e.origin = glm::vec3(btFrom.x(), btFrom.y(), btFrom.z());

    if (result.hasHit())
    {
        btVector3 hp = result.m_hitPointWorld;
        e.end   = glm::vec3(hp.x(), hp.y(), hp.z());
        e.color = glm::vec3(HIT_R, HIT_G, HIT_B);

        btVector3 n  = result.m_hitNormalWorld;
        glm::vec3 nv = glm::vec3(n.x(), n.y(), n.z());
        e.hasNormal   = true;
        e.normalStart = e.end;
        e.normalEnd   = e.end + nv * NORMAL_LENGTH;
    }
    else
    {
        e.end       = glm::vec3(btTo.x(), btTo.y(), btTo.z());
        e.color     = glm::vec3(MISS_R, MISS_G, MISS_B);
        e.hasNormal = false;
    }

    rays().push_back(e);
}

void DebugDrawRay::drawAndFlush()
{
    if (rays().empty()) return;

    // Save relevant state
    glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    for (const DebugRayEntry& e : rays())
    {
        // Ray line
        glColor3f(e.color.r, e.color.g, e.color.b);
        glVertex3f(e.origin.x, e.origin.y, e.origin.z);
        glVertex3f(e.end.x,    e.end.y,    e.end.z);

        // Normal (if present)
        if (e.hasNormal)
        {
            glColor3f(NORM_R, NORM_G, NORM_B);
            glVertex3f(e.normalStart.x, e.normalStart.y, e.normalStart.z);
            glVertex3f(e.normalEnd.x,   e.normalEnd.y,   e.normalEnd.z);
        }
    }
    glEnd();

    glPopAttrib();

    // ── Auto-clear for next frame ──
    rays().clear();
}

void DebugDrawRay::clear()
{
    rays().clear(); 
}
