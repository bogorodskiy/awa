#include <algorithm>
#include "Camera.h"

const float Camera::MIN_DISTANCE_FROM_TARGET = 2.0f;
const float Camera::MAX_DISTANCE_FROM_TARGET = 3.0f;

Camera::Camera():
    m_target(nullptr),
    m_rotationH(0.0f),
    m_currentPosition(0.0f, 0.0f, 0.0f),
    m_destPosition(0.0f, 0.0f, 0.0f),
    m_currentDirection(0.0f, 0.0f, 0.0f),
    m_destDirection(0.0f, 0.0f, 0.0f),
    m_currentUpVector(0.0f, 0.0f, 0.0f),
    m_destUpVector(0.0f, 0.0f, 0.0f)
{
}

const physx::PxVec3& Camera::getPosition() const{
    return m_destPosition;
}

void Camera::setPosition(float x, float y, float z) {
    m_currentPosition.x = x;
    m_currentPosition.y = y;
    m_currentPosition.z = z;

    m_destPosition.x = x;
    m_destPosition.y = y;
    m_destPosition.z = z;
}

const physx::PxVec3& Camera::getDirection() const{
    return m_destDirection;
}

void Camera::setDirection(float x, float y, float z) {
    m_currentDirection.x = x;
    m_currentDirection.y = y;
    m_currentDirection.z = z;

    m_destDirection.x = x;
    m_destDirection.y = y;
    m_destDirection.z = z;
}

const physx::PxVec3& Camera::getUpVector() const{
    return m_destUpVector;
}

void Camera::setUpVector(float x, float y, float z) {
    m_currentUpVector.x = x;
    m_currentUpVector.y = y;
    m_currentUpVector.z = z;

    m_destUpVector.x = x;
    m_destUpVector.y = y;
    m_destUpVector.z = z;
}

void Camera::setRoomBounds(float minX, float maxX,
                           float minY, float maxY,
                           float minZ, float maxZ) {
    m_roomBounds[0] = minX;
    m_roomBounds[1] = maxX;
    m_roomBounds[2] = minY;
    m_roomBounds[3] = maxY;
    m_roomBounds[4] = minZ;
    m_roomBounds[5] = maxZ;
}

void Camera::setTarget(GameObject* target) {
    m_target = target;
}

void Camera::rotateH(float value) {
    m_rotationH += value;
    // normalize to 0 - 360
    m_rotationH = m_rotationH - std::floor(m_rotationH / 360.0f) * 360.0f;
}

void Camera::update(float dt) {
    const auto& actorPosition = m_target->getActor()->getGlobalPose().p;
    const auto radians = glm::radians(m_rotationH);
    auto cameraX = actorPosition.x + glm::sin(radians) * 5.0f;
    auto cameraZ = actorPosition.z + glm::cos(radians) * 5.0f;

    //auto cameraPositionX = std::max(actorPosition.x, m_roomBounds[0]);
    //cameraPositionX = std::min(cameraPositionX, m_roomBounds[1]);

    auto cameraPositionY = std::max(actorPosition.y + 3.0f, m_roomBounds[2]);
    cameraPositionY = std::min(cameraPositionY, m_roomBounds[3]);

    //auto cameraPositionZ = std::max(actorPosition.z + 5.0f, m_roomBounds[4] - 10.0f);
    //cameraPositionZ = std::min(cameraPositionZ, m_roomBounds[5] + 10.0f);

    setPosition(cameraX, cameraPositionY , cameraZ);
    setDirection(actorPosition.x, actorPosition.y, actorPosition.z);
}