
#ifndef AWA_RIGIDDYNAMICCOMPONENT_H
#define AWA_RIGIDDYNAMICCOMPONENT_H

#include "PxRigidDynamic.h"
#include "../GameObjectComponent.h"

class RigidDynamicComponent: public GameObjectComponent {
public:
    static const physx::PxU32 NUM_SHAPES;
    static const physx::PxReal DENSITY;
    static const physx::PxReal ANGULAR_DAMPING;
    static const physx::PxReal LINEAR_DAMPING;

    void fillWith(physx::PxPhysics* pxPhysics,
                  physx::PxScene* pxScene,
                  physx::PxMaterial* material,
                  physx::PxSphereGeometry geometry,
                  GameObject* gameObject);
    void fillWith(const RigidDynamicComponent& source);
    void reset(bool finalize);
    ~RigidDynamicComponent();

    void addForce(physx::PxVec3 force);
    void update(float dt);
private:
    physx::PxRigidDynamic* m_pxActor = nullptr;
    physx::PxShape* m_pxShape = nullptr;
};


#endif //AWA_RIGIDDYNAMICCOMPONENT_H
