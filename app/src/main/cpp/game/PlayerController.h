
#ifndef AWA_PLAYERCONTROLLER_H
#define AWA_PLAYERCONTROLLER_H


#include <memory>
#include "systems/physics/RigidDynamicComponent.h"
#include "systems/input/TouchLayer.h"
#include "Camera.h"

class PlayerController {
public:
    PlayerController();
    void initialize(TouchLayer* touchLayer, Camera* camera);
    void setPawn(std::shared_ptr<RigidDynamicComponent> pawn);
    void update();

private:
    TouchLayer* m_touchLayer = nullptr;
    Camera* m_camera = nullptr;
    std::shared_ptr<RigidDynamicComponent> m_pawn;

    TouchInputHandler m_moveHandler;
    TouchInputHandler m_cameraHandler;
    TouchInputHandler m_actionHandler;
};


#endif //AWA_PLAYERCONTROLLER_H
