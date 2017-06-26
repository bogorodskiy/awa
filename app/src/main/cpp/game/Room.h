#ifndef AWA_ROOM_H
#define AWA_ROOM_H

#include <utility>
#include <vector>
#include "geometry/Geometry.h"
#include "PxPhysicsAPI.h"

class Room {
    using a_Side = std::pair<Geometry*, physx::PxRigidActor*>;

public:
    Room();
    ~Room();

    void connect(physx::PxPhysics* physics, physx::PxScene* scene);
    void initialize(float width, float height, float depth);
    std::vector<a_Side>& getElements();
    physx::PxVec3 getBounds();
private:

    std::vector<a_Side> m_sidePairs;
    physx::PxVec3 m_bounds;

    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;

    physx::PxRigidActor* createPlane(const physx::PxVec3& position, const physx::PxQuat& quat, const physx::PxMaterial* material);
};


#endif //AWA_ROOM_H
