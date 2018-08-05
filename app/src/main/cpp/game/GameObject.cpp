#include <PxPhysicsAPI.h>
#include "GameObject.h"

GameObject::GameObject(int id) :
    m_id(id)
{
    transform.p = physx::PxVec3(0.0f, 0.0f, 0.0f);
    transform.q = physx::PxQuat(0.0f, 0.0f, 0.0f, 1.0f);
}

GameObject::~GameObject() {
}

int GameObject::getId() const{
    return m_id;
}