#pragma once
#include <stack>
#include <memory>
#include "GameState.h"

class StateMachine {
private:
    std::stack<std::unique_ptr<GameState>> states;
    std::unique_ptr<GameState> newState; 
    bool isReplacing;

public:
    StateMachine() : isReplacing(false) {}

    
    void PushState(std::unique_ptr<GameState> state, bool replace = true) {
        isReplacing = replace;
        newState = std::move(state);
    }

    
    void PopState() {
        if (!states.empty()) {
            states.top()->OnExit();
            states.pop();
        }
        if (!states.empty()) {
            states.top()->OnEnter(); 
        }
    }

    void ProcessStateChanges() {
        if (newState) {
            if (isReplacing && !states.empty()) {
                states.top()->OnExit();
                states.pop();
            }
            else if (!states.empty()) {
                states.top()->OnExit(); 
            }

            states.push(std::move(newState));
            states.top()->OnEnter();
            isReplacing = false;
        }
    }

    void Update(float deltaTime) {
        if (!states.empty()) {
            states.top()->Update(deltaTime);
        }
    }

    void Render() {
        if (!states.empty()) {
            states.top()->Render();
        }
    }
};