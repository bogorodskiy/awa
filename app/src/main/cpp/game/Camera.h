#ifndef AWA_CAMERA_H
#define AWA_CAMERA_H

#include "foundation/PxVec3.h"
#include "foundation/PxMat44.h"
#include "GameObject.h"

class Camera {
public:
    Camera();

    const physx::PxVec3& getPosition() const;
    void setPosition(float x, float y, float z);

    const physx::PxVec3& getDirection() const;
    void setDirection(float x, float y, float z);

    const physx::PxVec3& getUpVector() const;
    void setUpVector(float x, float y, float z);

    physx::PxMat44 getProjectionViewMatrix();
    void setAspectRatio(float value);
    void setTarget(GameObject* target);
    void setRoomBounds(float minX, float maxX,
                       float minY, float maxY,
                       float minZ, float maxZ);

    void rotateH(float degree);
    void update(float dt);
private:
    static const float H_DISTANCE_FROM_TARGET;
    static const float V_DISTANCE_FROM_TARGET;
    static const float V_ANGLE_TAN;
    static const float DEGREES_TO_RADIANS;

    bool m_rotationChanged = false;
    GameObject* m_target = nullptr;
    float m_rotationH;
    physx::PxVec3 m_currentPosition;
    physx::PxVec3 m_destPosition;
    physx::PxVec3 m_currentDirection;
    physx::PxVec3 m_destDirection;
    physx::PxVec3 m_currentUpVector;
    physx::PxVec3 m_destUpVector;
    physx::PxVec3 m_lastActorPosition;
    physx::PxMat44 m_viewMatrix;
    physx::PxMat44 m_projectionMatrix;
    float m_roomBounds[6];
};


#endif //AWA_CAMERA_H
