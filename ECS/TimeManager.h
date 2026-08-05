#pragma once
#include <chrono>

class TimeManager {
private:
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    
    float unscaledDeltaTime; // temps réel
    float timeScale;         // Multiplicateur 

public:
    TimeManager() : unscaledDeltaTime(0.0f), timeScale(1.0f) {
        lastFrameTime = std::chrono::high_resolution_clock::now();
    }

    void Tick() {
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = currentFrameTime - lastFrameTime;
        
        unscaledDeltaTime = duration.count();
        lastFrameTime = currentFrameTime;

        // Limite si le jeu freeze
        if (unscaledDeltaTime > 0.1f) unscaledDeltaTime = 0.1f; 
    }

    // Le temps à utiliser pour la physique et les mouvements
    float GetDeltaTime() const { return unscaledDeltaTime * timeScale; }
    
    // Le temps réel pour l'UI
    float GetUnscaledDeltaTime() const { return unscaledDeltaTime; }

    void SetTimeScale(float scale) { timeScale = scale; }
    float GetTimeScale() const { return timeScale; }
};