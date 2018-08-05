#ifndef AWA_GAMEOBJECTCOMPONENT_H
#define AWA_GAMEOBJECTCOMPONENT_H

class GameObject;
#include "../GameObject.h"

class GameObjectComponent {
public:
    GameObjectComponent();
    virtual ~GameObjectComponent(){};
    GameObjectComponent(GameObject* gameObject);
    GameObject* getGameObject();
protected:
    GameObject* m_gameObject = nullptr;
};


#endif //AWA_GAMEOBJECTCOMPONENT_H
