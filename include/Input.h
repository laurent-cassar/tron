#pragma once

#include <windows.h>

class Input
{
public:
    static void initialize();
    static void update();
    
    static bool isKeyDown(int key);
    static bool isKeyPressed(int key);
    static bool isMouseButtonDown(int button);
    
    static float getMouseX();
    static float getMouseY();
    static float getMouseDeltaX();
    static float getMouseDeltaY();

private:
    static bool m_keys[256];
    static bool m_keysPressed[256];
    static bool m_mouseButtons[3];
    static float m_mouseX, m_mouseY;
    static float m_mouseDeltaX, m_mouseDeltaY;
    static float m_lastMouseX, m_lastMouseY;
};