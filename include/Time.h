#pragma once

class Time
{
public:
    static void initialize();
    static void update();
    
    static float deltaTime();
    static float totalTime();
    static float timeScale();
    static void setTimeScale(float scale);

private:
    static float m_deltaTime;
    static float m_totalTime;
    static float m_timeScale;
    static unsigned long long m_lastFrame;
};