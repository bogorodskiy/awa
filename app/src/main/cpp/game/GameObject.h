#ifndef IRON_BALLS_GAMEOBJECT_H
#define IRON_BALLS_GAMEOBJECT_H

class GameObjectComponent;
#include "PxTransform.h"
#include "systems/GameObjectComponent.h"

class GameObject {
public:
    bool transformChanged = true;
    physx::PxTransform transform;

    GameObject(int id, int health);
    ~GameObject();

    int getId() const;
    int getHealth() const;
    void addDamage(int value);
private:
    int m_id = -1;
    int m_health = 0;
};


#endif //IRON_BALLS_GAMEOBJECT_H
