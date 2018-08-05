
#ifndef AWA_RIGIDDYNAMICCOMPONENT_H
#define AWA_RIGIDDYNAMICCOMPONENT_H


#include "../GameObjectComponent.h"

class RigidDynamicComponent: public GameObjectComponent {
public:
    static const physx::PxU32 NUM_SHAPES;
    static const physx::PxReal DENSITY;
    static const physx::PxReal ANGULAR_DAMPING;
    static const physx::PxReal LINEAR_DAMPING;

    RigidDynamicComponent(physx::PxPhysics* pxPhysics,
                          physx::PxScene* pxScene,
                          physx::PxMaterial* material,
                          GameObject* gameObject);
    ~RigidDynamicComponent();

    void addForce(physx::PxVec3 force);
    physx::PxVec3 getGravity() const;
    void setGravity(float x, float y, float z);
    void update(float dt);
private:
    physx::PxRigidDynamic* m_pxActor = nullptr;
    physx::PxShape* m_pxShape = nullptr;
    physx::PxVec3 m_gravity;
};


#endif //AWA_RIGIDDYNAMICCOMPONENT_H
