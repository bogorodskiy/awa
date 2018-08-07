#ifndef AWA_RIGIDSTATICCOMPONENT_H
#define AWA_RIGIDSTATICCOMPONENT_H

#include "PxRigidStatic.h"
#include "../GameObjectComponent.h"

class RigidStaticComponent: public GameObjectComponent {

public:
    void fillWith(physx::PxPhysics* pxPhysics,
                  physx::PxScene* pxScene,
                  physx::PxMaterial* material,
                  physx::PxGeometry geometry,
                  GameObject* gameObject);
    void fillWith(const RigidStaticComponent& source);
    void reset(bool finalize);
    ~RigidStaticComponent();
private:
    physx::PxRigidActor* m_pxActor = nullptr;
};


#endif //AWA_RIGIDSTATICCOMPONENT_H
