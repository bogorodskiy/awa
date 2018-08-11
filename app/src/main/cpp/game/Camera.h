#ifndef AWA_CAMERA_H
#define AWA_CAMERA_H

#include "foundation/PxVec3.h"
#include "foundation/PxMat44.h"
#include "GameObject.h"

class Camera {
public:
    Camera();

    const physx::PxVec3& getPosition() const;
    const physx::PxVec3& getDirection() const;
    const physx::PxVec3& getUpVector() const;
    const physx::PxMat44& getProjectionViewMatrix() const;

    void setAspectRatio(float value);
    void setTarget(GameObject* target);

    void rotate(float pitch, float yaw);
    void update();
private:
    static const float DISTANCE_FROM_TARGET;
    static const float DEGREES_TO_RADIANS;

    bool m_rotationChanged = false;
    GameObject* m_target = nullptr;
    float m_yawRotation;
    float m_pitchRotation;

    physx::PxVec3 m_position;
    physx::PxVec3 m_direction;
    physx::PxVec3 m_upVector;
    physx::PxVec3 m_lastActorPosition;
    physx::PxMat44 m_projectionMatrix;
    physx::PxMat44 m_projectionViewMatrix;

    void updateProjectionViewMatrix();
};


#endif //AWA_CAMERA_H
