#include "Level.h"

Level::Level()
{
}

// TODO: clear/release objects
Level::~Level() {
}

void Level::initialize(float width, float height, float depth) {
    m_bounds.x = width;
    m_bounds.y = height;
    m_bounds.z = depth;

    auto lightPosition = physx::PxVec3{2.0f, 1.0f, 6.0f};
    auto lightColor = physx::PxVec3{1.0f, 1.0f, 1.0f};
    auto lightCenter = createPointLight(lightPosition, lightColor.x, lightColor.y, lightColor.z);
    m_pointLights.emplace_back(lightCenter);
}

physx::PxVec3 Level::getBounds() {
    return m_bounds;
}

// TODO: to raw ptr?
const std::vector<std::shared_ptr<PointLight>>& Level::getPointLights() const {
    return m_pointLights;
}

std::shared_ptr<PointLight> Level::createPointLight(const physx::PxVec3& position, float r, float g, float b) {
    auto result = std::make_shared<PointLight>();
    result->position.x = position.x;
    result->position.y = position.y;
    result->position.z = position.z;
    result->color.r = r;
    result->color.g = g;
    result->color.b = b;

    result->attenuation.constantFactor = 0.0f;
    result->attenuation.exponentialFactor = 0.0f;
    result->attenuation.linearFactor = 1.5f;
    result->diffuseIntensity = 0.2f;
    result->ambientIntensity = 5.0f;

    return result;
}