#ifndef AWA_CAMERA_H
#define AWA_CAMERA_H

#include <foundation/PxVec4.h>
#include <foundation/PxVec2.h>
#include "GameObject.h"

class Camera {
public:
    Camera();
    void setTarget(GameObject* target);
    const physx::PxVec3& getPosition() const;
    void setPosition(float x, float y, float z);
    const physx::PxVec3& getDirection() const;
    void setDirection(float x, float y, float z);
    const physx::PxVec3& getUpVector() const;
    void setUpVector(float x, float y, float z);
    void setRoomBounds(float minX, float maxX,
                       float minY, float maxY,
                       float minZ, float maxZ);
    void rotateH(float degree);
    void update(float dt);
private:
    static const float H_DISTANCE_FROM_TARGET;
    static const float V_DISTANCE_FROM_TARGET;
    static const float V_ANGLE_TAN;

    bool m_rotationChanged;
    GameObject* m_target;
    float m_rotationH;
    physx::PxVec3 m_currentPosition;
    physx::PxVec3 m_destPosition;
    physx::PxVec3 m_currentDirection;
    physx::PxVec3 m_destDirection;
    physx::PxVec3 m_currentUpVector;
    physx::PxVec3 m_destUpVector;
    physx::PxVec3 m_lastActorPosition;
    float m_roomBounds[6];
};


#endif //AWA_CAMERA_H
