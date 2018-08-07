#include "RigidStaticComponent.h"
#include "PxPhysicsAPI.h"

RigidStaticComponent::~RigidStaticComponent() {
    reset(true);
}

void RigidStaticComponent::fillWith(physx::PxPhysics* pxPhysics,
                                    physx::PxScene* pxScene,
                                    physx::PxMaterial* material,
                                    physx::PxGeometry geometry,
                                    GameObject* gameObject)
{
    m_gameObject = gameObject;
    m_pxActor = pxPhysics->createRigidStatic(gameObject->transform);
    auto shape = pxPhysics->createShape(geometry, *material);
    m_pxActor->attachShape(*shape);
    m_pxActor->userData = m_gameObject;
    pxScene->addActor(*m_pxActor);
}

void RigidStaticComponent::fillWith(const RigidStaticComponent& source) {
    m_gameObject = source.m_gameObject;
    m_pxActor = source.m_pxActor;
}

void RigidStaticComponent::reset(bool finalize) {
    if (finalize && m_pxActor != nullptr) {
        m_pxActor->release();
    }
}