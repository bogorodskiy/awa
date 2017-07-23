#ifndef AWA_ROOM_H
#define AWA_ROOM_H

#include <utility>
#include <vector>
#include "geometry/Geometry.h"
#include "PxPhysicsAPI.h"
#include "light/PointLight.h"
#include <memory>

class Room {
    using a_Side = std::pair<Geometry*, physx::PxRigidActor*>;

public:
    Room();
    ~Room();

    void connect(physx::PxPhysics* physics, physx::PxScene* scene);
    void initialize(float width, float height, float depth);
    std::vector<a_Side>& getElements();
    physx::PxVec3 getBounds();
    const std::vector<std::shared_ptr<PointLight>>& getPointLights() const;
private:

    std::vector<a_Side> m_sidePairs;
    physx::PxVec3 m_bounds;

    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;
    std::vector<std::shared_ptr<PointLight>> m_pointLights;

    physx::PxRigidActor* createPlane(const physx::PxVec3& position, const physx::PxQuat& quat, const physx::PxMaterial* material);
    std::shared_ptr<PointLight> createPointLight(const physx::PxVec3& position, float r, float g, float b);
};


#endif //AWA_ROOM_H
