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

    auto floor = GeometryCache::getInstance()->getPlane();
    floor->setColor(0.6f, 0.6f, 0.6f, 1.0f);
    auto floorRotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    auto floorPx = createPlane({0.0f, 0.0f, 0.0f}, floorRotation, material);
    m_pxScene->addActor(*floorPx);
    m_sidePairs.emplace_back(std::make_pair(floor, floorPx));
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