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

void Room::initialize() {
    physx::PxMaterial* material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);

    auto roomWidth = 20.0f;
    auto roomHeight = 10.0f;
    auto roomDepth = 30.0f;
    auto roomHalfWidth = roomWidth * 0.5f;
    auto roomHalfHeight = roomHeight * 0.5f;
    auto roomHalfDepth = roomDepth * 0.5f;

    auto plane = GeometryCache::getInstance()->getPlane(roomDepth, roomWidth);
    plane->setColor(0.6f, 0.6f, 0.6f, 1.0f);
    auto floorRotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    auto floorPx = createPlane({0.0f, 0.0f, 0.0f}, floorRotation, material);
    m_pxScene->addActor(*floorPx);
    m_sidePairs.emplace_back(std::make_pair(plane, floorPx));

    plane = GeometryCache::getInstance()->getPlane(roomWidth, roomHeight);
    plane->setColor(0.2f, 0.2f, 0.2f, 1.0f);
    auto frontWallRotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    auto frontWallPx = createPlane({0.0f, roomHalfHeight, roomHalfDepth}, frontWallRotation, material);
    m_pxScene->addActor(*frontWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, frontWallPx));

    plane = GeometryCache::getInstance()->getPlane(roomWidth, roomHeight);
    plane->setColor(0.5f, 0.5f, 0.5f, 1.0f);
    auto backWallRotation = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    auto backWallPx = createPlane({0.0f, roomHalfHeight, -roomHalfDepth}, backWallRotation, material);
    m_pxScene->addActor(*backWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, backWallPx));

    plane = GeometryCache::getInstance()->getPlane(roomDepth, roomHeight);
    plane->setColor(0.1f, 0.1f, 0.1f, 1.0f);
    auto leftWallRotation = physx::PxQuat(0, physx::PxVec3(0.0f, 0.0f, 0.0f));
    auto leftWallPx = createPlane({-roomHalfWidth, roomHalfHeight, 0.0f}, leftWallRotation, material);
    m_pxScene->addActor(*leftWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, leftWallPx));

    plane = GeometryCache::getInstance()->getPlane(roomDepth, roomHeight);
    plane->setColor(0.7f, 0.7f, 0.7f, 1.0f);
    auto rightWallRotation = physx::PxQuat(physx::PxPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    auto rightWallPx = createPlane({roomHalfWidth, roomHalfHeight, 0.0f}, rightWallRotation, material);
    m_pxScene->addActor(*rightWallPx);
    m_sidePairs.emplace_back(std::make_pair(plane, rightWallPx));

    plane = GeometryCache::getInstance()->getPlane(roomDepth, roomWidth);
    plane->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto ceilingRotation = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    auto ceilingPx = createPlane({0.0f, roomHeight, 0.0f}, ceilingRotation, material);
    m_pxScene->addActor(*ceilingPx);
    m_sidePairs.emplace_back(std::make_pair(plane, ceilingPx));
}

std::vector<Room::a_Side>& Room::getElements() {
    return m_sidePairs;
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
    return plane;
}