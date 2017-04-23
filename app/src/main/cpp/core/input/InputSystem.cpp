#include "InputSystem.h"

InputSystem::InputSystem():
        m_lastFreeIndex(-1),
        m_numAddedComponents(0),
        m_touchLayer(nullptr)
{

}

void InputSystem::initialize(TouchLayer *touchLayer) {
    m_components = std::make_unique<InputComponent[]>(MAX_ENTITIES);

    m_touchLayer = touchLayer;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        m_components[i].connect(touchLayer);
    }
}

void InputSystem::update(float deltaTime) {
    for (int i = 0; i < m_numAddedComponents; i++) {
        auto& component = m_components[i];
        if (component.active) {
            component.update(deltaTime);
        }
    }
}

void InputSystem::addEntity(GameObject* entity) {
    if (m_numAddedComponents == MAX_ENTITIES && m_lastFreeIndex == -1) {
        LOGD("Cannot add entity to InputSystem, max entities count reached");
        return;
    }

    if (m_lastFreeIndex >= 0) {
        m_components[m_freeIndices[m_lastFreeIndex]].setEntity(entity);
        m_lastFreeIndex--;
    }
    else {
        m_components[m_numAddedComponents].setEntity(entity);
        m_numAddedComponents++;
    }
}

void InputSystem::clearComponent(int index) {
    if (m_lastFreeIndex < MAX_ENTITIES - 1) {
        m_lastFreeIndex++;
        m_freeIndices[m_lastFreeIndex] = index;
    }
    else {
        LOGD("Cannot clear component, free indices out of bounds");
    }
    auto& component = m_components[index];
    if (component.active) {
        component.releaseEntity();
    }
}

void InputSystem::removeAllEntities() {
    for (int i = 0; i < m_numAddedComponents; i++) {
        auto& component = m_components[i];
        if (component.active) {
            component.releaseEntity();
        }
    }
    m_numAddedComponents = 0;
    m_lastFreeIndex = -1;
}
