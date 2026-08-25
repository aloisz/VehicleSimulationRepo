#pragma once

#include <iostream>
#include <string>
#include <GL/freeglut.h>

namespace Color
{
    // ANSI console logging
    inline const char* Reset = "\033[0m";
    inline const char* Gray = "\033[90m";
    inline const char* Cyan = "\033[36m";
    inline const char* Yellow = "\033[33m";
    inline const char* Red = "\033[31m";
    inline const char* White = "\033[37m";

    // RGB for OpenGL
    struct RGB { float r, g, b; };

    inline constexpr RGB RGB_Gray = { 0.5f, 0.5f, 0.5f };
    inline constexpr RGB RGB_Cyan = { 0.0f, 1.0f, 1.0f };
    inline constexpr RGB RGB_Yellow = { 1.0f, 1.0f, 0.0f };
    inline constexpr RGB RGB_Red = { 1.0f, 0.2f, 0.2f };
    inline constexpr RGB RGB_Green = { 0.2f, 1.0f, 0.3f };
    inline constexpr RGB RGB_White = { 1.0f, 1.0f, 1.0f };

    inline void ApplyGL(RGB color)
    {
        glColor3f(color.r, color.g, color.b);
    }
}