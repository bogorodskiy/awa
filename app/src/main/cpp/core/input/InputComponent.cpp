#include "InputComponent.h"

InputComponent::InputComponent():
        active(false),
        index(-1),
        m_moveHandler(TouchInputHandler::STICK, 0),
        m_gameObject(nullptr),
        m_touchLayer(nullptr)

{

}

void InputComponent::connect(TouchLayer* touchLayer) {
    m_touchLayer = touchLayer;

    // TODO logic for other commands like shooting, jumping etc
    // TODO height from touch layer or const
    m_moveHandler.updateBounds(600, 1080, ScreenBounds::AlignHorizontal::LEFT, ScreenBounds::AlignVertical::TOP);
    m_touchLayer->addTouchHandler(&m_moveHandler);
}

void InputComponent::setEntity(GameObject *gameObject) {
    m_gameObject = gameObject;
    active = true;
}

void InputComponent::update(float deltaTime) {
    if (m_moveHandler.getActive()) {
        m_gameObject->setMoveDirection(m_moveHandler.getDirectionX(), -m_moveHandler.getDirectionY());
    }
    else{
        m_gameObject->setMoveDirection(0.0f, 0.0f);
    }
}

void InputComponent::releaseEntity() {
    if (m_touchLayer != nullptr) {
        m_touchLayer->removeTouchHandler(&m_moveHandler);
    }
    m_gameObject = nullptr;
    active = false;
}
