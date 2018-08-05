#include "GameObjectComponent.h"

GameObjectComponent::GameObjectComponent() {
}

GameObjectComponent::GameObjectComponent(GameObject* gameObject): m_gameObject(gameObject) {
}

GameObject* GameObjectComponent::getGameObject() {
    return m_gameObject;
}