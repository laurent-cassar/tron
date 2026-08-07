#include "Time.h"
#include <windows.h>

float Time::m_deltaTime = 0.0f;
float Time::m_totalTime = 0.0f;
float Time::m_timeScale = 1.0f;
unsigned long long Time::m_lastFrame = 0;

void Time::initialize()
{
    m_lastFrame = GetTickCount64();
}

void Time::update()
{
    unsigned long long now = GetTickCount64();
    m_deltaTime = (now - m_lastFrame) / 1000.0f * m_timeScale;
    m_lastFrame = now;
    m_totalTime += m_deltaTime;
}

float Time::deltaTime() { return m_deltaTime; }
float Time::totalTime() { return m_totalTime; }
float Time::timeScale() { return m_timeScale; }
void Time::setTimeScale(float scale) { m_timeScale = scale; }