#include <algorithm>
#include <cmath>
#include "foundation/PxVec2.h"
#include "Camera.h"
#include "GlobalSettings.h"
#include "../core/common.h"

const float Camera::DISTANCE_FROM_TARGET = 5.0f;
const float Camera::DEGREES_TO_RADIANS = 0.01745329251994329576923690768489f;

Camera::Camera():
    m_yawRotation(0.0f),
    m_pitchRotation(0.0f),
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, 0.0f),
    m_upVector(0.0f, 0.0f, 0.0f),
    m_lastActorPosition(0.0f, 0.0f, 0.0f)
{
    m_upVector.x = 0.0f;
    m_upVector.y = 1.0f;
    m_upVector.z = 0.0f;
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
    if (m_pitchRotation > 60) {
        m_pitchRotation = 60;
    }
    if (m_pitchRotation < -30) {
        m_pitchRotation = -30;
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

    const auto cX = -std::cos(yawRadians) * std::cos(pitchRadians) * DISTANCE_FROM_TARGET;
    const auto cY = std::sin(pitchRadians) * DISTANCE_FROM_TARGET;
    const auto cZ = std::sin(yawRadians) * std::cos(pitchRadians) * DISTANCE_FROM_TARGET;

    m_position = m_target->transform.p + physx::PxVec3(cX, cY, cZ) ;

    m_direction = actorPosition - m_position;
    m_direction.normalizeFast();
}

void Camera::updateProjectionViewMatrix() {
    // look at implementation
    auto s = m_direction.cross(m_upVector).getNormalized();
    auto u = s.cross(m_direction);
    auto viewMatrix = physx::PxMat44{1.0f};
    viewMatrix[0][0] = s.x;
    viewMatrix[1][0] = s.y;
    viewMatrix[2][0] = s.z;
    viewMatrix[0][1] = u.x;
    viewMatrix[1][1] = u.y;
    viewMatrix[2][1] = u.z;
    viewMatrix[0][2] = -m_direction.x;
    viewMatrix[1][2] = -m_direction.y;
    viewMatrix[2][2] = -m_direction.z;
    viewMatrix[3][0] = -s.dot(m_position);
    viewMatrix[3][1] = -u.dot(m_position);
    viewMatrix[3][2] =  m_direction.dot(m_position);

    m_projectionViewMatrix = m_projectionMatrix * viewMatrix;
}