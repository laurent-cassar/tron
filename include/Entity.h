#pragma once

#include <vector>
#include <memory>
#include <typeinfo>
#include "Component.h"
#include "Transform.h"

class Entity
{
public:
    Entity();
    virtual ~Entity();
    
    template<typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        comp->m_entity = this;
        m_components.push_back(std::move(comp));
        return ptr;
    }
    
    template<typename T>
    T* getComponent() const
    {
        for (auto& comp : m_components)
        {
            T* ptr = dynamic_cast<T*>(comp.get());
            if (ptr) return ptr;
        }
        return nullptr;
    }
    
    void update(float deltaTime);
    void render();
    
    Transform* transform() { return &m_transform; }
    const Transform* transform() const { return &m_transform; }
    
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

private:
    Transform m_transform;
    std::vector<std::unique_ptr<Component>> m_components;
    bool m_active = true;
};