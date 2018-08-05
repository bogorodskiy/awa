#ifndef IRON_BALLS_GAMEOBJECT_H
#define IRON_BALLS_GAMEOBJECT_H

class GameObjectComponent;
#include <PxRigidDynamic.h>
#include "geometry/Geometry.h"
#include "systems/GameObjectComponent.h"

class GameObject {
public:
    physx::PxTransform transform;
    Geometry* geometry = nullptr;

    // TODO: add hp

    GameObject(int id);
    ~GameObject();

    int getId() const;
private:
    int m_id;
};


#endif //IRON_BALLS_GAMEOBJECT_H
