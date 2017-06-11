#include "GameObject.h"
#include "../../glm/gtx/transform.hpp"

GameObject::GameObject(int id, Geometry* geometry, physx::PxRigidDynamic* actor) :
    m_id(id),
    m_geometry(geometry),
    m_physxActor(actor)
{
}

GameObject::~GameObject() {
    if (m_physxActor != nullptr) {
        m_physxActor->release();
    }
}

int GameObject::getId() const{
    return m_id;
}

Geometry* GameObject::getGeometry() const{
    return m_geometry;
}

physx::PxRigidDynamic* GameObject::getActor() const {
    return m_physxActor;
}

void GameObject::setPosition(float x, float y, float z) {
    if (m_physxActor != nullptr) {
        auto currentTransform = m_physxActor->getGlobalPose();
        currentTransform.p.x = x;
        currentTransform.p.y = x;
        currentTransform.p.z = x;
        m_physxActor->setGlobalPose(currentTransform);
    }
}

void GameObject::addForce(float x, float y, float z) {
    physx::PxVec3 force{x, y, z};
    m_physxActor->addForce(force, physx::PxForceMode::eFORCE);
}

void GameObject::addImpulse(float x, float y, float z) {
    physx::PxVec3 force{x, y, z};
    m_physxActor->addForce(force, physx::PxForceMode::eIMPULSE);
}