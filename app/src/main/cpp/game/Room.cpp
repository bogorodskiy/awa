#include "Room.h"
#include "geometry/GeometryCache.h"

Room::Room():
        m_pxPhysics(nullptr),
        m_pxScene(nullptr)
{
}

// TODO: clear/release objects
Room::~Room() {

}

void Room::connect(physx::PxPhysics* physics, physx::PxScene* scene) {
    m_pxPhysics = physics;
    m_pxScene = scene;
}

void Room::initialize(float width, float height, float depth) {
    physx::PxMaterial* material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);

    m_bounds.x = width;
    m_bounds.y = height;
    m_bounds.z = depth;

    auto halfWidth = width * 0.5f;
    auto halfHeight = height * 0.5f;
    auto halfDepth = depth * 0.5f;

    auto plane = GeometryCache::getInstance()->getPlane(depth, width);
    plane->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto floorRotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    auto floorPx = createPlane({0.0f, 0.0f, 0.0f}, floorRotation, material);
    m_pxScene->addActor(*floorPx);
    m_sidePairs.emplace_back(std::make_pair(plane, floorPx));

    plane = GeometryCache::getInstance()->getPlane(width, height);
    plane->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto frontWallRotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    auto frontWallPx = createPlane({0.0f, halfHeight, halfDepth}, frontWallRotation, material);
    m_pxScene->addActor(*frontWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, frontWallPx));

    plane = GeometryCache::getInstance()->getPlane(width, height);
    plane->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto backWallRotation = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    auto backWallPx = createPlane({0.0f, halfHeight, -halfDepth}, backWallRotation, material);
    m_pxScene->addActor(*backWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, backWallPx));

    plane = GeometryCache::getInstance()->getPlane(depth, height);
    plane->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto leftWallRotation = physx::PxQuat(0, physx::PxVec3(0.0f, 0.0f, 0.0f));
    auto leftWallPx = createPlane({-halfWidth, halfHeight, 0.0f}, leftWallRotation, material);
    m_pxScene->addActor(*leftWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, leftWallPx));

    plane = GeometryCache::getInstance()->getPlane(depth, height);
    plane->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto rightWallRotation = physx::PxQuat(physx::PxPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    auto rightWallPx = createPlane({halfWidth, halfHeight, 0.0f}, rightWallRotation, material);
    m_pxScene->addActor(*rightWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, rightWallPx));

    plane = GeometryCache::getInstance()->getPlane(depth, width);
    plane->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto ceilingRotation = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    auto ceilingPx = createPlane({0.0f, height, 0.0f}, ceilingRotation, material);
    m_pxScene->addActor(*ceilingPx);
    m_sidePairs.emplace_back(std::make_pair(plane, ceilingPx));

//    auto lightCenter = createPointLight({0.0f, 3.0f, 0.0f}, 1.0f, 1.0f, 1.0f);
//    m_pointLights.emplace_back(lightCenter);

    auto light1 = createPointLight({-halfWidth + 1.0f, halfHeight - 1.0f, -halfDepth + 1.0f}, 1.0f, 0.0f, 0.0f);
    m_pointLights.emplace_back(light1);

    auto light2 = createPointLight({halfWidth - 1.0f, halfHeight - 1.0f, -halfDepth + 1.0f}, 0.0f, 1.0f, 0.0f);
    m_pointLights.emplace_back(light2);

    auto light3 = createPointLight({-halfWidth + 1.0f, halfHeight - 1.0f, halfDepth - 1.0f}, 0.0f, 0.0f, 1.0f);
    m_pointLights.emplace_back(light3);

    auto light4 = createPointLight({halfWidth - 1.0f, halfHeight - 1.0f, halfDepth - 1.0f}, 1.0f, 1.0f, 0.0f);
    m_pointLights.emplace_back(light4);
}

std::vector<Room::a_Side>& Room::getElements() {
    return m_sidePairs;
}

physx::PxVec3 Room::getBounds() {
    return m_bounds;
}

const std::vector<std::shared_ptr<PointLight>>& Room::getPointLights() const {
    return m_pointLights;
}

physx::PxRigidActor* Room::createPlane(const physx::PxVec3& position,
                                       const physx::PxQuat& quat,
                                       const physx::PxMaterial* material) {
    physx::PxTransform pose = physx::PxTransform(position, quat);
    physx::PxRigidStatic* plane = m_pxPhysics->createRigidStatic(pose);
    if (!plane) {
        LOGI("Create plane failed!!");
        return nullptr;
    }

    auto planeShape = m_pxPhysics->createShape(physx::PxPlaneGeometry(), *material);
    if (!planeShape) {
        LOGI("Create plane shape failed!");
        return nullptr;
    }
    plane->attachShape(*planeShape);
    plane->userData = nullptr;
    return plane;
}

std::shared_ptr<PointLight> Room::createPointLight(const physx::PxVec3& position, float r, float g, float b) {
    auto result = std::make_shared<PointLight>();
    result->position.x = position.x;
    result->position.y = position.y;
    result->position.z = position.z;
    result->color.r = r;
    result->color.g = g;
    result->color.b = b;

    result->attenuation.linearFactor = 0.1f;
    result->diffuseIntensity = 0.5f;
    //result->ambientIntensity = 0.2f;

    return result;
}