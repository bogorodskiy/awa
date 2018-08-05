#ifndef AWA_RIGIDSTATICCOMPONENT_H
#define AWA_RIGIDSTATICCOMPONENT_H

#include "../GameObjectComponent.h"

class RigidStaticComponent: public GameObjectComponent {

public:
    RigidStaticComponent(physx::PxPhysics* pxPhysics,
                         physx::PxScene* pxScene,
                         physx::PxMaterial* material,
                         GameObject* gameObject);
    ~RigidStaticComponent();
private:
    physx::PxRigidActor* m_pxActor = nullptr;
};


#endif //AWA_RIGIDSTATICCOMPONENT_H
