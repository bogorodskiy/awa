#include <algorithm>
#include <cmath>
#include "foundation/PxVec2.h"
#include "Camera.h"
#include "GlobalSettings.h"
#include "../core/common.h"

const float Camera::H_DISTANCE_FROM_TARGET = 5.0f;
const float Camera::V_DISTANCE_FROM_TARGET = 3.0f;
const float Camera::DEGREES_TO_RADIANS = 0.01745329251994329576923690768489f;

Camera::Camera():
    m_yawRotation(0.0f),
    m_pitchRotation(0.0f),
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, 0.0f),
    m_upVector(0.0f, 0.0f, 0.0f),
    m_lastActorPosition(0.0f, 0.0f, 0.0f)
{
}

const physx::PxVec3& Camera::getPosition() const{
    return m_position;
}

const physx::PxVec3& Camera::getDirection() const{
    return m_direction;
}

const physx::PxVec3& Camera::getUpVector() const{
    return m_upVector;
}

void Camera::setLevelBounds(float minX, float maxX,
                            float minY, float maxY,
                            float minZ, float maxZ) {
    m_levelBounds[0] = minX;
    m_levelBounds[1] = maxX;
    m_levelBounds[2] = minY;
    m_levelBounds[3] = maxY;
    m_levelBounds[4] = minZ;
    m_levelBounds[5] = maxZ;
}

const physx::PxMat44& Camera::getProjectionViewMatrix() const {
    return m_projectionViewMatrix;
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

    updateProjectionViewMatrix();
}

void Camera::setTarget(GameObject* target) {
    m_target = target;
}

void Camera::rotate(float pitch, float yaw) {
    m_pitchRotation += pitch;
    m_yawRotation += yaw;

    // limit rotations
    if (m_pitchRotation > 0) {
        m_pitchRotation = 0;
    }
    if (m_pitchRotation < -89) {
        m_pitchRotation = -89;
    }
    while (m_yawRotation < -180) {
        m_yawRotation += 360;
    }
    while (m_yawRotation > 180) {
        m_yawRotation -= 360;
    }

    m_rotationChanged = true;
}

void Camera::update() {
    const auto& actorPosition = m_target->transform.p;
    if (actorPosition == m_lastActorPosition && !m_rotationChanged) {
        return;
    }
    m_lastActorPosition = actorPosition;
    m_rotationChanged = false;
    updateProjectionViewMatrix();

    const auto pitchRadians = m_pitchRotation * DEGREES_TO_RADIANS;
    const auto yawRadians = m_yawRotation * DEGREES_TO_RADIANS;
    auto cX = std::sin(yawRadians) * H_DISTANCE_FROM_TARGET;
    auto cY = -std::sin(pitchRadians) * V_DISTANCE_FROM_TARGET;
    auto cZ = std::cos(yawRadians) * H_DISTANCE_FROM_TARGET;

    m_position = m_target->transform.p + physx::PxVec3(cX, cY, cZ) ;

    m_direction = actorPosition;

    m_upVector.x = 0.0f;
    m_upVector.y = 1.0f;
    m_upVector.z = 0.0f;
}

void Camera::updateProjectionViewMatrix() {
    // look at implementation
    auto f = (m_direction - m_position).getNormalized();
    auto s = f.cross(m_upVector).getNormalized();
    auto u = s.cross(f);
    auto viewMatrix = physx::PxMat44{1.0f};
    viewMatrix[0][0] = s.x;
    viewMatrix[1][0] = s.y;
    viewMatrix[2][0] = s.z;
    viewMatrix[0][1] = u.x;
    viewMatrix[1][1] = u.y;
    viewMatrix[2][1] = u.z;
    viewMatrix[0][2] = -f.x;
    viewMatrix[1][2] = -f.y;
    viewMatrix[2][2] = -f.z;
    viewMatrix[3][0] = -s.dot(m_position);
    viewMatrix[3][1] = -u.dot(m_position);
    viewMatrix[3][2] =  f.dot(m_position);

    m_projectionViewMatrix = m_projectionMatrix * viewMatrix;
}