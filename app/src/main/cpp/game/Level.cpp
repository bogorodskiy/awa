#include "Level.h"

physx::PxVec3 Level::getBounds() {
    return m_bounds;
}

const std::vector<std::shared_ptr<PointLight>>& Level::getPointLights() const {
    return m_pointLights;
}

int Level::initializeLevel(GraphicsSystem& graphicsSystem, PhysicsSystem& physicsSystem, int lastFreeId) {
    const auto levelWidth = 20.0f;
    const auto levelHeight = 10.0f;
    const auto levelDepth = 30.0f;
    m_bounds.x = levelWidth;
    m_bounds.y = levelHeight;
    m_bounds.z = levelDepth;

    auto halfWidth = levelWidth * 0.5f;
    auto halfHeight = levelHeight * 0.5f;
    auto halfDepth = levelDepth * 0.5f;
    auto whiteColor = physx::PxVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // floor
    auto plane = new GameObject(lastFreeId++, 0);
    auto size = physx::PxVec3(levelDepth, levelWidth, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, 0.0f, 0.0f);
    plane->transform.q = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    graphicsSystem.addEntity(plane,
                             Geometry::Type::PLANE,
                             size,
                             whiteColor);
    physicsSystem.addStaticEntity(plane);

    // frontWall
    plane = new GameObject(lastFreeId++, 0);
    size = physx::PxVec3(levelWidth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, halfHeight, halfDepth);
    plane->transform.q = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    graphicsSystem.addEntity(plane,
                             Geometry::Type::PLANE,
                             size,
                             whiteColor);
    physicsSystem.addStaticEntity(plane
    );

    // backWall
    plane = new GameObject(lastFreeId++, 0);
    size = physx::PxVec3(levelWidth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, halfHeight, -halfDepth);
    plane->transform.q = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    graphicsSystem.addEntity(plane,
                             Geometry::Type::PLANE,
                             size,
                             whiteColor);
    physicsSystem.addStaticEntity(plane);

    // leftWall
    plane = new GameObject(lastFreeId++, 0);
    size = physx::PxVec3(levelDepth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(-halfWidth, halfHeight, 0.0f);
    plane->transform.q = physx::PxQuat(0, physx::PxVec3(0.0f, 0.0f, 0.0f));
    graphicsSystem.addEntity(plane,
                             Geometry::Type::PLANE,
                             size,
                             whiteColor);
    physicsSystem.addStaticEntity(plane);

    // rightWall
    plane = new GameObject(lastFreeId++, 0);
    size = physx::PxVec3(levelDepth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(halfWidth, halfHeight, 0.0f);
    plane->transform.q = physx::PxQuat(physx::PxPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    graphicsSystem.addEntity(plane,
                             Geometry::Type::PLANE,
                             size,
                             whiteColor);
    physicsSystem.addStaticEntity(plane);

    // ceiling
    plane = new GameObject(lastFreeId++, 0);
    size = physx::PxVec3(levelDepth, levelWidth, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, levelHeight, 0.0f);
    plane->transform.q = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    graphicsSystem.addEntity(plane,
                             Geometry::Type::PLANE,
                             size,
                             whiteColor);
    physicsSystem.addStaticEntity(plane);

    auto lightPosition = physx::PxVec3{2.0f, 1.0f, 6.0f};
    auto lightColor = physx::PxVec3{1.0f, 1.0f, 1.0f};
    auto lightCenter = createPointLight(lightPosition, lightColor.x, lightColor.y, lightColor.z);
    m_pointLights.emplace_back(lightCenter);

    return lastFreeId;
}

std::shared_ptr<PointLight> Level::createPointLight(const physx::PxVec3& position, float r, float g, float b) {
    auto result = std::make_shared<PointLight>();
    result->position.x = position.x;
    result->position.y = position.y;
    result->position.z = position.z;
    result->color.x = r;
    result->color.y = g;
    result->color.z = b;

    result->attenuation.constantFactor = 0.0f;
    result->attenuation.exponentialFactor = 0.0f;
    result->attenuation.linearFactor = 1.5f;
    result->diffuseIntensity = 0.2f;
    result->ambientIntensity = 5.0f;

    return result;
}