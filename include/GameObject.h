#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Component.h"

class GameObject {
private:
    std::string name;
    bool isActive;
    std::vector<std::unique_ptr<Component>> components;

public:
    GameObject(const std::string& name = "GameObject") : name(name), isActive(true) {}
    ~GameObject() = default;

    // Ajouter un composant
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        // Crée le composant
        std::unique_ptr<T> newComponent = std::make_unique<T>(std::forward<Args>(args)...);
        newComponent->SetGameObject(this);
        T* rawPtr = newComponent.get();
        
        components.push_back(std::move(newComponent));
        rawPtr->Awake(); // Initialisation
        return rawPtr;
    }

    // Récupérer UN composant du type demandé
    template <typename T>
    T* GetComponent() {
        for (auto& comp : components) {
            T* ptr = dynamic_cast<T*>(comp.get());
            if (ptr != nullptr) {
                return ptr;
            }
        }
        return nullptr;
    }

    // Récupérer TOUS les composants du type demandé
    template <typename T>
    std::vector<T*> GetComponents() {
        std::vector<T*> result;
        for (auto& comp : components) {
            T* ptr = dynamic_cast<T*>(comp.get());
            if (ptr != nullptr) {
                result.push_back(ptr);
            }
        }
        return result;
    }

    void Update(float deltaTime) {
        if (!isActive) return;
        for (auto& comp : components) {
            comp->Update(deltaTime);
        }
    }

    void Render() {
        if (!isActive) return;
        for (auto& comp : components) {
            comp->Render();
        }
    }
};