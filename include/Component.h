#pragma once

class Entity;

class Component
{
public:
    virtual ~Component() = default;
    virtual void update(float deltaTime) {}
    virtual void render() {}
    
    Entity* getEntity() const { return m_entity; }

protected:
    Entity* m_entity = nullptr;
    friend class Entity;
};