#ifndef IRON_BALLS_INPUTCOMPONENT_H
#define IRON_BALLS_INPUTCOMPONENT_H

#include "../../core/gameplay/GameObject.h"
#include "TouchInputHandler.h"
#include "TouchLayer.h"

class InputComponent {
public:
    InputComponent();

    bool active;
    int index;

    void connect(TouchLayer* touchLayer);
    void setEntity(GameObject* gameObject);
    void update(float deltaTime);
    void releaseEntity();
private:
    TouchInputHandler m_moveHandler;
    TouchInputHandler m_actionHandler;

    GameObject* m_gameObject;
    TouchLayer* m_touchLayer;
};


#endif //IRON_BALLS_INPUTCOMPONENT_H
