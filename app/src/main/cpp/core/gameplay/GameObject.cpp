#include <PxPhysicsAPI.h>
#include "GameObject.h"
#include "../../glm/gtx/transform.hpp"

GameObject::GameObject(int id, Geometry* geometry, physx::PxRigidDynamic* actor, physx::PxShape** shapeBuffer) :
    m_id(id),
    m_geometry(geometry),
    m_pxActor(actor),
    m_pxShape(nullptr),
    m_gravity(0.0f, -9.8f, 0.0f)
{
    m_pxActor->userData = this;

    physx::PxU32 nShapes = actor->getNbShapes();
    if (nShapes > 1) {
        LOGI("ERROR: Actor has more than one shape");
        return;
    }

    actor->getShapes(shapeBuffer, nShapes);
    while (nShapes--)
    {
        m_pxShape = shapeBuffer[nShapes];
    }
}

GameObject::~GameObject() {
    if (m_pxActor != nullptr) {
        m_pxActor->userData = nullptr;
        m_pxActor->release();
    }
}

int GameObject::getId() const{
    return m_id;
}

Geometry* GameObject::getGeometry() const{
    return m_geometry;
}

physx::PxRigidDynamic* GameObject::getActor() const {
    return m_pxActor;
}

physx::PxShape* GameObject::getPxShape() const {
    return m_pxShape;
}

void GameObject::setPosition(float x, float y, float z) {
    if (m_pxActor != nullptr) {
        auto currentTransform = m_pxActor->getGlobalPose();
        currentTransform.p.x = x;
        currentTransform.p.y = x;
        currentTransform.p.z = x;
        m_pxActor->setGlobalPose(currentTransform);
    }
}

void GameObject::addForce(physx::PxVec3 force) {
    m_pxActor->addForce(force, physx::PxForceMode::eFORCE);
}

void GameObject::addImpulse(physx::PxVec3 force) {
    m_pxActor->addForce(force, physx::PxForceMode::eIMPULSE);
}

void GameObject::applyGravity() {
    addForce(m_gravity);
}