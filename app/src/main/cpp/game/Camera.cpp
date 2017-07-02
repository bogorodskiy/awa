#include <algorithm>
#include "Camera.h"

const float Camera::H_DISTANCE_FROM_TARGET = 5.0f;
const float Camera::V_DISTANCE_FROM_TARGET = 3.0f;
const float Camera::V_ANGLE_TAN = Camera::V_DISTANCE_FROM_TARGET / Camera::H_DISTANCE_FROM_TARGET;

Camera::Camera():
    m_rotationChanged(false),
    m_target(nullptr),
    m_rotationH(0.0f),
    m_currentPosition(0.0f, 0.0f, 0.0f),
    m_destPosition(0.0f, 0.0f, 0.0f),
    m_currentDirection(0.0f, 0.0f, 0.0f),
    m_destDirection(0.0f, 0.0f, 0.0f),
    m_currentUpVector(0.0f, 0.0f, 0.0f),
    m_destUpVector(0.0f, 0.0f, 0.0f),
    m_lastActorPosition(0.0f, 0.0f, 0.0f)
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

void Camera::rotateH(float degree) {
    m_rotationH += degree;
    // normalize to 0 - 360
    static const auto one_div_threesixty = 1 / 360.0f;
    m_rotationH = m_rotationH - std::floor(m_rotationH * one_div_threesixty) * 360.0f;
    m_rotationChanged = true;
}

void Camera::update(float dt) {
    const auto& actorPosition = m_target->getActor()->getGlobalPose().p;
    if (actorPosition == m_lastActorPosition && !m_rotationChanged) {
        return;
    }
    m_lastActorPosition = actorPosition;
    m_rotationChanged = false;

    const auto radians = glm::radians(m_rotationH);
    auto cameraX = actorPosition.x + glm::sin(radians) * H_DISTANCE_FROM_TARGET;
    auto cameraZ = actorPosition.z + glm::cos(radians) * H_DISTANCE_FROM_TARGET;

    physx::PxVec2 actorHPos{actorPosition.x, actorPosition.z};
    physx::PxVec2 cameraPos{cameraX, cameraZ};
    auto direction = cameraPos - actorHPos;
    direction.normalize();

    auto isOutOfBounds = false;
    auto minX = m_roomBounds[0];
    auto maxX = m_roomBounds[1];
    auto distanceX = H_DISTANCE_FROM_TARGET;
    if (cameraX < minX || cameraX > maxX) {
        auto pointX = (cameraX < minX) ? minX - actorPosition.x : maxX - actorPosition.x;
        auto divisor = (direction.x != 0) ? direction.x : 1.0f;
        distanceX = std::abs(pointX / divisor);
        isOutOfBounds = true;
    }

    auto minZ = m_roomBounds[4];
    auto maxZ = m_roomBounds[5];
    auto distanceZ = H_DISTANCE_FROM_TARGET;
    if (cameraZ < minZ || cameraZ > maxZ) {
        auto pointZ = (cameraZ < minZ) ? minZ - actorPosition.z : maxZ - actorPosition.z;
        auto divisor = (direction.y != 0) ? direction.y : 1.0f;
        distanceZ = std::abs(pointZ / divisor);
        isOutOfBounds = true;
    }

    auto yDelta = V_DISTANCE_FROM_TARGET;
    if (isOutOfBounds) {
        auto distance = std::min(distanceX, distanceZ);
        yDelta = distance * V_ANGLE_TAN;

        cameraX = actorPosition.x + distance * direction.x;
        cameraZ = actorPosition.z + distance * direction.y;
    }

    auto minY = m_roomBounds[2];
    auto maxY = m_roomBounds[3];
    auto cameraY = std::max(actorPosition.y + yDelta, minY);
    cameraY = std::min(cameraY, maxY);

    setPosition(cameraX, cameraY , cameraZ);
    setDirection(actorPosition.x, actorPosition.y, actorPosition.z);
}