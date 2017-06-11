#include "InputComponent.h"

InputComponent::InputComponent():
        active(false),
        index(-1),
        m_moveHandler(TouchInputHandler::TouchInputType::STICK, 0),
        m_actionHandler(TouchInputHandler::TouchInputType::BUTTON, 1),
        m_gameObject(nullptr),
        m_touchLayer(nullptr)

{

}

void InputComponent::connect(TouchLayer* touchLayer) {
    m_touchLayer = touchLayer;

    // TODO height from touch layer or const
    m_moveHandler.updateBounds(600, 1080, ScreenBounds::AlignHorizontal::LEFT, ScreenBounds::AlignVertical::TOP);
    m_touchLayer->addTouchHandler(&m_moveHandler);
    m_actionHandler.updateBounds(600, 1080, ScreenBounds::AlignHorizontal::RIGHT, ScreenBounds::AlignVertical::TOP);
    m_touchLayer->addTouchHandler(&m_actionHandler);
}

void InputComponent::setEntity(GameObject *gameObject) {
    m_gameObject = gameObject;
    active = true;
}

void InputComponent::update(float deltaTime) {
    if (m_moveHandler.getActive()) {
        auto amplifier = 5.0f;
        m_gameObject->addForce(m_moveHandler.getDirectionX() * amplifier, 0.0f, -m_moveHandler.getDirectionY() * amplifier);
    }
    if (m_actionHandler.getAndResetTouched()) {
        m_gameObject->addImpulse(0.0f, 4.0f, 0.0f);
    }
}

void InputComponent::releaseEntity() {
    if (m_touchLayer != nullptr) {
        m_touchLayer->removeTouchHandler(&m_moveHandler);
    }
    m_gameObject = nullptr;
    active = false;
}
