#ifndef AWA_ROOM_H
#define AWA_ROOM_H

#include <utility>
#include <vector>
#include "PxPhysicsAPI.h"
#include "systems/graphics/light/PointLight.h"
#include <memory>

class Level {
public:
    Level();
    ~Level();

    void initialize(float width, float height, float depth);
    physx::PxVec3 getBounds();
    const std::vector<std::shared_ptr<PointLight>>& getPointLights() const;
private:

    physx::PxVec3 m_bounds;
    std::vector<std::shared_ptr<PointLight>> m_pointLights;
    std::shared_ptr<PointLight> createPointLight(const physx::PxVec3& position, float r, float g, float b);
};


#endif //AWA_ROOM_H
