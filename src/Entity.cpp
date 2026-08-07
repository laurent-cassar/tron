#include "Entity.h"

Entity::Entity() {}
Entity::~Entity() {}

void Entity::update(float deltaTime)
{
    if (!m_active) return;
    for (auto& comp : m_components)
        comp->update(deltaTime);
}

void Entity::render()
{
    if (!m_active) return;
    for (auto& comp : m_components)
        comp->render();
}