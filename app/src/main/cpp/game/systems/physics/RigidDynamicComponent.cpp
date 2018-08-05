#include "RigidDynamicComponent.h"
#include "PxPhysicsAPI.h"

const physx::PxU32 RigidDynamicComponent::NUM_SHAPES = 1;
const physx::PxReal RigidDynamicComponent::DENSITY = 1.0;
const physx::PxReal RigidDynamicComponent::ANGULAR_DAMPING = 0.5;
const physx::PxReal RigidDynamicComponent::LINEAR_DAMPING = 0.5;

RigidDynamicComponent::RigidDynamicComponent(physx::PxPhysics* pxPhysics,
                                             physx::PxScene* pxScene,
                                             physx::PxMaterial* material,
                                             GameObject* gameObject):
        GameObjectComponent(gameObject)
{
    // TODO: m_gravity(0.0f, -9.8f, 0.0f)
    m_gravity.x = 0.0f;
    m_gravity.y = -9.8f;
    m_gravity.z = 0.0f;
    // Create sphere
    // TODO: size and geometry type from gameObject->geometry
    physx::PxSphereGeometry pxGeometry(0.5);
    m_pxActor = PxCreateDynamic(*pxPhysics, gameObject->transform, pxGeometry, *material, DENSITY);
    // damping - friction or resistance, angular = rotation/spinning, linear = moving
    m_pxActor->setAngularDamping(ANGULAR_DAMPING);
    m_pxActor->setLinearDamping(LINEAR_DAMPING);
    m_pxActor->setLinearVelocity(physx::PxVec3(0,0,0));
    pxScene->addActor(*m_pxActor);

    physx::PxShape* pxShapeBuffer[NUM_SHAPES];
    m_pxActor->getShapes(pxShapeBuffer, NUM_SHAPES);
    m_pxShape = pxShapeBuffer[0];
}

RigidDynamicComponent::~RigidDynamicComponent() {
    if (m_pxActor != nullptr) {
        // releases all shapes
        m_pxActor->release();
    }
}

void RigidDynamicComponent::addForce(physx::PxVec3 force) {
    m_pxActor->addForce(force, physx::PxForceMode::eFORCE);
}

physx::PxVec3 RigidDynamicComponent::getGravity() const {
    return m_gravity;
}

void RigidDynamicComponent::setGravity(float x, float y, float z) {
    m_gravity.x = x;
    m_gravity.y = y;
    m_gravity.z = z;
}

void RigidDynamicComponent::update(float dt) {
    addForce(m_gravity);
    m_gameObject->transform = physx::PxShapeExt::getGlobalPose(*m_pxShape, *m_pxActor);
}