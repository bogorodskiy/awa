#ifndef IRON_BALLS_GAMEOBJECT_H
#define IRON_BALLS_GAMEOBJECT_H

#include <PxRigidDynamic.h>
#include "../../game/geometry/Geometry.h"

class GameObject {
public:
    GameObject(int id, Geometry* geometry, physx::PxRigidDynamic* actor);
    ~GameObject();

    int getId() const;
    Geometry* getGeometry() const;
    physx::PxRigidDynamic* getActor() const;
    void setPosition(float x, float y, float z);
    void addForce(float x, float y, float z);
    void addImpulse(float x, float y, float z);
private:
    unsigned int m_id;
    Geometry* m_geometry;
    physx::PxRigidDynamic* m_physxActor;
};


#endif //IRON_BALLS_GAMEOBJECT_H
