#include "Input.h"

#include <cctype>

namespace Input
{
    Input& Input::Get()
    {
        static Input instance;
        return instance;
    }

    void Input::OnKeyDown(unsigned char key)
    {
        const unsigned char k = Normalize(key);

        if (!_held[k]) 
        {
            _pressedThisFrame[k] = true;
        }

        _held[k] = true;
    }

    void Input::OnKeyUp(unsigned char key)
    {
        _held[Normalize(key)] = false;
    }

    void Input::OnSpecialDown(int key)
    {
        if (key >= 0 && key < SPECIAL_COUNT)
            _specialHeld[key] = true;
    }

    void Input::OnSpecialUp(int key)
    {
        if (key >= 0 && key < SPECIAL_COUNT)
            _specialHeld[key] = false;
    }

    bool Input::IsKeyHeld(unsigned char key)
    {
        return _held[Normalize(key)];
    }

    bool Input::WasKeyPressed(unsigned char key)
    {
        return _pressedThisFrame[Normalize(key)];
    }

    bool Input::IsSpecialHeld(int key) const
    {
        return false;
    }

    void Input::ClearFrame()
    {
        _pressedThisFrame.fill(false);
    }

    unsigned char Input::Normalize(unsigned char key)
    {
        return static_cast<unsigned char>(std::tolower(key));
    }
}

