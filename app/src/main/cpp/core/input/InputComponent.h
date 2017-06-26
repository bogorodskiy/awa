#ifndef IRON_BALLS_INPUTCOMPONENT_H
#define IRON_BALLS_INPUTCOMPONENT_H

#include "../../core/gameplay/GameObject.h"
#include "TouchInputHandler.h"
#include "TouchLayer.h"
#include "../../game/Camera.h"

class InputComponent {
public:
    InputComponent();

    bool active;
    int index;

    void connect(TouchLayer* touchLayer, Camera* camera);
    void setEntity(GameObject* gameObject);
    void update(float dt);
    void releaseEntity();
private:
    TouchInputHandler m_moveHandler;
    TouchInputHandler m_cameraHandler;
    TouchInputHandler m_actionHandler;

    GameObject* m_gameObject;
    TouchLayer* m_touchLayer;
    Camera* m_camera;
};


#endif //IRON_BALLS_INPUTCOMPONENT_H
