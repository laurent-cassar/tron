#include "Input.h"

bool Input::m_keys[256] = {};
bool Input::m_keysPressed[256] = {};
bool Input::m_mouseButtons[3] = {};
float Input::m_mouseX = 0.0f;
float Input::m_mouseY = 0.0f;
float Input::m_mouseDeltaX = 0.0f;
float Input::m_mouseDeltaY = 0.0f;
float Input::m_lastMouseX = 0.0f;
float Input::m_lastMouseY = 0.0f;

void Input::initialize()
{
    for (int i = 0; i < 256; ++i)
    {
        m_keys[i] = false;
        m_keysPressed[i] = false;
    }
}

void Input::update()
{
    for (int i = 0; i < 256; ++i)
    {
        bool isDown = GetAsyncKeyState(i) & 0x8000;
        m_keysPressed[i] = isDown && !m_keys[i];
        m_keys[i] = isDown;
    }
    
    POINT p;
    GetCursorPos(&p);
    m_mouseX = static_cast<float>(p.x);
    m_mouseY = static_cast<float>(p.y);
    m_mouseDeltaX = m_mouseX - m_lastMouseX;
    m_mouseDeltaY = m_mouseY - m_lastMouseY;
    m_lastMouseX = m_mouseX;
    m_lastMouseY = m_mouseY;
    
    m_mouseButtons[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    m_mouseButtons[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    m_mouseButtons[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
}

bool Input::isKeyDown(int key)
{
    return m_keys[key & 0xFF];
}

bool Input::isKeyPressed(int key)
{
    return m_keysPressed[key & 0xFF];
}

bool Input::isMouseButtonDown(int button)
{
    return button >= 0 && button < 3 && m_mouseButtons[button];
}

float Input::getMouseX() { return m_mouseX; }
float Input::getMouseY() { return m_mouseY; }
float Input::getMouseDeltaX() { return m_mouseDeltaX; }
float Input::getMouseDeltaY() { return m_mouseDeltaY; }