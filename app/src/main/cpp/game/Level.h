#ifndef AWA_ROOM_H
#define AWA_ROOM_H

class GraphicsSystem;
class PhysicsSystem;

#include <utility>
#include <vector>
#include "PxPhysicsAPI.h"
#include "systems/graphics/light/PointLight.h"
#include "systems/graphics/GraphicsSystem.h"
#include "systems/physics/PhysicsSystem.h"
#include <memory>

class Level {
public:
    physx::PxVec3 getBounds();
    const std::vector<std::shared_ptr<PointLight>>& getPointLights() const;
    int initializeLevel(GraphicsSystem& graphicsSystem, PhysicsSystem& physicsSystem, int lastFreeId);
private:
    physx::PxVec3 m_bounds;
    std::vector<std::shared_ptr<PointLight>> m_pointLights;
    std::shared_ptr<PointLight> createPointLight(const physx::PxVec3& position, float r, float g, float b);
};


#endif //AWA_ROOM_H
