#include "Primitives.h"

#include <GL/freeglut.h>
#include <cmath>

namespace Primitives
{
    void DrawBox(float width, float height, float depth)
    {
        const float w = width * 0.5f;
        const float h = height * 0.5f;
        const float d = depth * 0.5f;

        glBegin(GL_QUADS);

        glNormal3f(0, 0, 1);
        glVertex3f(-w, -h, d); glVertex3f(w, -h, d);
        glVertex3f(w, h, d);   glVertex3f(-w, h, d);

        glNormal3f(0, 0, -1);
        glVertex3f(-w, -h, -d); glVertex3f(-w, h, -d);
        glVertex3f(w, h, -d);   glVertex3f(w, -h, -d);

        glNormal3f(0, 1, 0);
        glVertex3f(-w, h, -d); glVertex3f(-w, h, d);
        glVertex3f(w, h, d);   glVertex3f(w, h, -d);

        glNormal3f(0, -1, 0);
        glVertex3f(-w, -h, -d); glVertex3f(w, -h, -d);
        glVertex3f(w, -h, d);   glVertex3f(-w, -h, d);

        glNormal3f(1, 0, 0);
        glVertex3f(w, -h, -d); glVertex3f(w, h, -d);
        glVertex3f(w, h, d);   glVertex3f(w, -h, d);

        glNormal3f(-1, 0, 0);
        glVertex3f(-w, -h, -d); glVertex3f(-w, -h, d);
        glVertex3f(-w, h, d);   glVertex3f(-w, h, -d);

        glEnd();
    }

    void DrawCylinderX(float radius, float width, int segments)
    {
        if (segments < 3) segments = 3;

        const float halfWidth = width * 0.5f;
        const float step = 2.0f * 3.14159265358979323846f / static_cast<float>(segments);

        // Tread
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segments; ++i)
        {
            const float a = step * static_cast<float>(i);
            const float y = std::cos(a);
            const float z = std::sin(a);

            glNormal3f(0.0f, y, z);
            glVertex3f(-halfWidth, y * radius, z * radius);
            glVertex3f(halfWidth, y * radius, z * radius);
        }
        glEnd();

        // Sidewalls
        for (int side = 0; side < 2; ++side)
        {
            const float x = (side == 0) ? -halfWidth : halfWidth;
            const float nx = (side == 0) ? -1.0f : 1.0f;

            glBegin(GL_TRIANGLE_FAN);
            glNormal3f(nx, 0.0f, 0.0f);
            glVertex3f(x, 0.0f, 0.0f);

            for (int i = 0; i <= segments; ++i)
            {
                // Wind the far side in reverse so both faces point outwards
                const int index = (side == 0) ? i : (segments - i);
                const float a = step * static_cast<float>(index);
                glVertex3f(x, std::cos(a) * radius, std::sin(a) * radius);
            }
            glEnd();
        }

        glDisable(GL_LIGHTING);
        glColor3f(0.65f, 0.65f, 0.68f);
        glBegin(GL_LINES);
        glVertex3f(halfWidth * 1.01f, 0.0f, 0.0f);
        glVertex3f(halfWidth * 1.01f, radius * 0.92f, 0.0f);
        glEnd();
        glEnable(GL_LIGHTING);
    }

    void DrawGrid(float halfSize, float spacing)
    {
        glDisable(GL_LIGHTING);
        glColor3f(0.25f, 0.27f, 0.30f);

        glBegin(GL_LINES);
        for (float v = -halfSize; v <= halfSize + 0.001f; v += spacing)
        {
            glVertex3f(v, 0.01f, -halfSize);
            glVertex3f(v, 0.01f, halfSize);

            glVertex3f(-halfSize, 0.01f, v);
            glVertex3f(halfSize, 0.01f, v);
        }
        glEnd();

        glEnable(GL_LIGHTING);
    }
}
