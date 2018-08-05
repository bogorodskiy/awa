#include "RigidStaticComponent.h"
#include "PxPhysicsAPI.h"

RigidStaticComponent::RigidStaticComponent(physx::PxPhysics* pxPhysics,
                                           physx::PxScene* pxScene,
                                           physx::PxMaterial* material,
                                           GameObject* gameObject): GameObjectComponent(gameObject)
{
    m_pxActor = pxPhysics->createRigidStatic(gameObject->transform);
    // TODO: get shape from game object
    auto shape = pxPhysics->createShape(physx::PxPlaneGeometry(), *material);
    m_pxActor->attachShape(*shape);
    pxScene->addActor(*m_pxActor);
}

RigidStaticComponent::~RigidStaticComponent() {
    if (m_pxActor != nullptr) {
        m_pxActor->release();
    }
}