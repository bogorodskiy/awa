#ifndef AWA_PHYSICSSYSTEM_H
#define AWA_PHYSICSSYSTEM_H


#include "../core/gameplay/GameObject.h"

class PhysicsSystem {
    void init();
    void addPlane();
    void addGameObject(GameObject* object);
};


#endif //AWA_PHYSICSSYSTEM_H
