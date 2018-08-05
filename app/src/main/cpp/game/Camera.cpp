#include <algorithm>
#include <cmath>
#include "foundation/PxVec2.h"
#include "Camera.h"
#include "GlobalSettings.h"

const float Camera::H_DISTANCE_FROM_TARGET = 5.0f;
const float Camera::V_DISTANCE_FROM_TARGET = 3.0f;
const float Camera::V_ANGLE_TAN = Camera::V_DISTANCE_FROM_TARGET / Camera::H_DISTANCE_FROM_TARGET;
const float Camera::DEGREES_TO_RADIANS = 0.01745329251994329576923690768489f;

Camera::Camera():
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
    return m_currentPosition;
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

physx::PxMat44 Camera::getProjectionViewMatrix() {
    // TODO: calculate when dirty, not in get

    // look at implementation
    auto f = (m_destDirection - m_destPosition).getNormalized();
    auto s = f.cross(m_destUpVector).getNormalized();
    auto u = s.cross(f);
    m_viewMatrix = physx::PxMat44{1.0f};
    m_viewMatrix[0][0] = s.x;
    m_viewMatrix[1][0] = s.y;
    m_viewMatrix[2][0] = s.z;
    m_viewMatrix[0][1] = u.x;
    m_viewMatrix[1][1] = u.y;
    m_viewMatrix[2][1] = u.z;
    m_viewMatrix[0][2] = -f.x;
    m_viewMatrix[1][2] = -f.y;
    m_viewMatrix[2][2] = -f.z;
    m_viewMatrix[3][0] = -s.dot(m_destPosition);
    m_viewMatrix[3][1] = -u.dot(m_destPosition);
    m_viewMatrix[3][2] =  f.dot(m_destPosition);

    return m_projectionMatrix * m_viewMatrix;
}

void Camera::setAspectRatio(float value) {

    // fill projection matrix
    auto rad = GlobalSettings::FIELD_OF_VIEW;
    auto tanHalfFovy = std::tan(rad * 0.5f);
    m_projectionMatrix = physx::PxMat44{0.0f};
    m_projectionMatrix[0][0] = 1 / (value * tanHalfFovy);
    m_projectionMatrix[1][1] = 1 / (tanHalfFovy);
    m_projectionMatrix[2][2] = - (GlobalSettings::FAR_CLIPPING + GlobalSettings::NEAR_CLIPPING) / (GlobalSettings::FAR_CLIPPING - GlobalSettings::NEAR_CLIPPING);
    m_projectionMatrix[2][3] = -1.0f;
    m_projectionMatrix[3][2] = - (2 * GlobalSettings::FAR_CLIPPING * GlobalSettings::NEAR_CLIPPING) / (GlobalSettings::FAR_CLIPPING - GlobalSettings::NEAR_CLIPPING);
}

void Camera::setTarget(GameObject* target) {
    m_target = target;
}

void Camera::rotateH(float degree) {
    // TODO: m_rotationH to radians
    m_rotationH += degree;
    // normalize to 0 - 360
    static const auto oneDivThreesixty = 1 / 360.0f;
    m_rotationH = m_rotationH - std::floor(m_rotationH * oneDivThreesixty) * 360.0f;
    m_rotationChanged = true;
}

void Camera::update(float dt) {
    const auto& actorPosition = m_target->transform.p;
    if (actorPosition == m_lastActorPosition && !m_rotationChanged) {
        return;
    }
    m_lastActorPosition = actorPosition;
    m_rotationChanged = false;

    const auto radians = m_rotationH * DEGREES_TO_RADIANS;
    auto cameraX = actorPosition.x + std::sin(radians) * H_DISTANCE_FROM_TARGET;
    auto cameraZ = actorPosition.z + std::cos(radians) * H_DISTANCE_FROM_TARGET;

    physx::PxVec2 actorHPos{actorPosition.x, actorPosition.z};
    physx::PxVec2 cameraPos{cameraX, cameraZ};
    auto direction = cameraPos - actorHPos;
    direction.normalize();

    auto isOutOfBounds = false;
    const auto minX = m_roomBounds[0];
    const auto maxX = m_roomBounds[1];
    auto distanceX = H_DISTANCE_FROM_TARGET;
    if (cameraX < minX || cameraX > maxX) {
        auto pointX = (cameraX < minX) ? minX - actorPosition.x : maxX - actorPosition.x;
        auto divisor = (direction.x != 0) ? direction.x : 1.0f;
        distanceX = std::abs(pointX / divisor);
        isOutOfBounds = true;
    }

    const auto minZ = m_roomBounds[4];
    const auto maxZ = m_roomBounds[5];
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