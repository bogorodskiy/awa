#ifndef IRON_BALLS_GAMEOBJECT_H
#define IRON_BALLS_GAMEOBJECT_H

#include <PxRigidDynamic.h>
#include "../../game/geometry/Geometry.h"

class GameObject {
public:
    GameObject(int id, Geometry* geometry, physx::PxRigidDynamic* actor, physx::PxShape** shapeBuffer);
    ~GameObject();

    int getId() const;
    Geometry* getGeometry() const;
    physx::PxRigidDynamic* getActor() const;
    physx::PxShape* getPxShape() const;
    void setPosition(float x, float y, float z);
    void addForce(physx::PxVec3 force);
    void addImpulse(physx::PxVec3 force);
    void applyGravity();
private:
    unsigned int m_id;
    Geometry* m_geometry;
    physx::PxRigidDynamic* m_pxActor;
    physx::PxShape* m_pxShape;
    physx::PxVec3 m_gravity;
};


#endif //IRON_BALLS_GAMEOBJECT_H
