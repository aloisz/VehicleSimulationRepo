#include "DebugDraw.h"

#include <GL/freeglut.h>

namespace
{
    struct ColoredLine
    {
        btVector3 From;
        btVector3 To;
        btVector3 Color;
    };

    bool g_enabled = true;
    std::vector<DebugDraw::Ray> g_rays;
    std::vector<ColoredLine> g_lines;

    constexpr size_t MAX_ENTRIES = 4096;
}

namespace DebugDraw
{
    void SetEnabled(bool enabled)
    {
        g_enabled = enabled;
        if (!enabled) Clear();
    }

    bool IsEnabled()
    {
        return g_enabled;
    }

    void AddRay(const btVector3& from, const btVector3& to, bool hit)
    {
        if (!g_enabled || g_rays.size() >= MAX_ENTRIES) return;
        g_rays.push_back(Ray{ from, to, hit });
    }

    void AddLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        if (!g_enabled || g_lines.size() >= MAX_ENTRIES) return;
        g_lines.push_back(ColoredLine{ from, to, color });
    }

    void Clear()
    {
        g_rays.clear();
        g_lines.clear();
    }

    void DrawAndFlush()
    {
        if (!g_enabled)
        {
            Clear();
            return;
        }

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glBegin(GL_LINES);

        for (const Ray& ray : g_rays)
        {
            if (ray.Hit)
            {
                Color::ApplyGL(Color::RGB_Green);
            }
            else
            {
                Color::ApplyGL(Color::RGB_Red);
            }

            glVertex3f(ray.From.getX(), ray.From.getY(), ray.From.getZ());
            glVertex3f(ray.To.getX(), ray.To.getY(), ray.To.getZ());
        }

        for (const ColoredLine& line : g_lines)
        {
            glColor3f(line.Color.getX(), line.Color.getY(), line.Color.getZ());
            glVertex3f(line.From.getX(), line.From.getY(), line.From.getZ());
            glVertex3f(line.To.getX(), line.To.getY(), line.To.getZ());
        }

        glEnd();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);

        Clear();
    }
}
