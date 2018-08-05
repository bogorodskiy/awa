#ifndef AWA_PHYSICSSYSTEM_H
#define AWA_PHYSICSSYSTEM_H

#include <vector>
#include "../../GameObject.h"
#include "../GameObjectComponent.h"
#include "RigidDynamicComponent.h"
#include "RigidStaticComponent.h"
#include "PxPhysicsAPI.h"
#include "../../geometry/GeometryFactory.h"

class PhysicsSystem: public physx::PxSimulationEventCallback {
public:
    static const physx::PxReal STATIC_FRICTION;
    static const physx::PxReal DYNAMIC_FRICTION;
    static const physx::PxReal RESTITUTION;

    PhysicsSystem();
    ~PhysicsSystem();
    bool initialize();

    std::shared_ptr<RigidDynamicComponent> getDynamicComponent(GameObject* gameObject);
    void addDynamicEntity(GameObject* gameObject,
                          GeometryFactory::GeometryType primitive,
                          physx::PxVec3 size);
    void addStaticEntity(GameObject* gameObject,
                         GeometryFactory::GeometryType primitive,
                         physx::PxVec3 size);
    void removeDynamicEntity(GameObject* gameObject);
    void removeStaticEntity(GameObject* gameObject);
    void update(float dt);

    // PxSimulationEventCallback
    virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {};
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {};
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {};
    virtual void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {};

    bool isPlayer(physx::PxRigidActor* actor);
    bool isPlane(physx::PxRigidActor* actor);

private:
    int m_numDynamicComponents = 0;
    int m_numStaticComponents = 0;
    physx::PxFoundation* m_pxFoundation;
    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;
    physx::PxMaterial* m_pxMaterial;

    std::vector<std::shared_ptr<RigidDynamicComponent>> m_dynamicComponents;
    std::vector<std::shared_ptr<RigidStaticComponent>> m_staticComponents;

    template<typename T>
    void removeEntity(std::vector<std::shared_ptr<T>>& container, int& numComponents, GameObject* gameObject) {
        auto position = 0u;
        for (; position < numComponents; ++position)
        {
            if (container[position]->getGameObject() == gameObject)
            {
                break;
            }
        }
        if (position == numComponents)
        {
            LOGE("Unable to find gameObject with id %i in Physics components", gameObject->getId());
            return;
        }

        if (numComponents > 1)
        {
            container[position] = container[numComponents - 1];
            container[numComponents - 1] = nullptr;
        }
        else {
            container[position] = nullptr;
        }
        --numComponents;
    }
};


#endif //AWA_PHYSICSSYSTEM_H
