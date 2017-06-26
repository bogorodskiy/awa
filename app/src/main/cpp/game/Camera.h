#ifndef AWA_CAMERA_H
#define AWA_CAMERA_H


#include <foundation/PxVec4.h>
#include "../core/gameplay/GameObject.h"

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
    void rotateH(float value);
    void update(float dt);
private:
    static const float MIN_DISTANCE_FROM_TARGET;
    static const float MAX_DISTANCE_FROM_TARGET;

    GameObject* m_target;
    float m_rotationH;
    physx::PxVec3 m_currentPosition;
    physx::PxVec3 m_destPosition;
    physx::PxVec3 m_currentDirection;
    physx::PxVec3 m_destDirection;
    physx::PxVec3 m_currentUpVector;
    physx::PxVec3 m_destUpVector;
    float m_roomBounds[6];
};


#endif //AWA_CAMERA_H
