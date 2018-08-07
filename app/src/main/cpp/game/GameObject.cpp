#include <algorithm>
#include "PxPhysicsAPI.h"
#include "GameObject.h"

GameObject::GameObject(int id, int health) :
    m_id(id),
    m_health(health)
{
    transform.p = physx::PxVec3(0.0f, 0.0f, 0.0f);
    transform.q = physx::PxQuat(0.0f, 0.0f, 0.0f, 1.0f);
}

GameObject::~GameObject() {
}

int GameObject::getId() const{
    return m_id;
}

int GameObject::getHealth() const {
    return m_health;
}

void GameObject::addDamage(int value) {
    m_health = std::max(0, m_health - value);
}