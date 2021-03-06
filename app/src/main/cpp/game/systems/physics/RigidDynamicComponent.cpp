#include "RigidDynamicComponent.h"
#include "PxPhysicsAPI.h"

const physx::PxReal RigidDynamicComponent::DENSITY = 1.0;
const physx::PxReal RigidDynamicComponent::ANGULAR_DAMPING = 0.5;
const physx::PxReal RigidDynamicComponent::LINEAR_DAMPING = 0.5;

RigidDynamicComponent::~RigidDynamicComponent() {
    reset(true);
}

void RigidDynamicComponent::fillWith(physx::PxPhysics* pxPhysics,
                                     physx::PxScene* pxScene,
                                     physx::PxShape* shape,
                                     GameObject* gameObject)
{
    m_gameObject = gameObject;
    m_pxShape = shape;
    m_pxActor = PxCreateDynamic(*pxPhysics, gameObject->transform, *shape, DENSITY);
    // damping - friction or resistance, angular = rotation/spinning, linear = moving
    m_pxActor->setAngularDamping(ANGULAR_DAMPING);
    m_pxActor->setLinearDamping(LINEAR_DAMPING);
    m_pxActor->setLinearVelocity(physx::PxVec3(0,0,0));
    m_pxActor->userData = m_gameObject;
    pxScene->addActor(*m_pxActor);
}

void RigidDynamicComponent::fillWith(const RigidDynamicComponent& source) {
    m_gameObject = source.m_gameObject;
    m_pxShape = source.m_pxShape;
    m_pxActor = source.m_pxActor;
}

void RigidDynamicComponent::addForce(physx::PxVec3 force) {
    m_pxActor->addForce(force, physx::PxForceMode::eFORCE);
}

void RigidDynamicComponent::update(float dt) {
    auto transform = physx::PxShapeExt::getGlobalPose(*m_pxShape, *m_pxActor);
    m_gameObject->transformChanged = !(m_gameObject->transform == transform);
    m_gameObject->transform = transform;
}

void RigidDynamicComponent::reset(bool finalize) {
    if (finalize && m_pxActor != nullptr) {
        m_pxActor->detachShape(*m_pxShape);
        m_pxActor->release();
    }
    m_gameObject = nullptr;
    m_pxShape = nullptr;
    m_pxActor = nullptr;
}