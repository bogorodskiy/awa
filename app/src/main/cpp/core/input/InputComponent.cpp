#include "InputComponent.h"

InputComponent::InputComponent():
        active(false),
        index(-1),
        m_moveHandler(TouchInputHandler::TouchInputType::STICK, 0),
        m_cameraHandler(TouchInputHandler::TouchInputType::STICK, 1),
        m_actionHandler(TouchInputHandler::TouchInputType::BUTTON, 2),
        m_gameObject(nullptr),
        m_touchLayer(nullptr),
        m_camera(nullptr)
{

}

void InputComponent::connect(TouchLayer* touchLayer, Camera* camera) {
    m_touchLayer = touchLayer;
    m_camera = camera;

    // TODO height from touch layer or const
    m_moveHandler.updateBounds(600, 1080, ScreenBounds::AlignHorizontal::LEFT, ScreenBounds::AlignVertical::TOP);
    m_touchLayer->addTouchHandler(&m_moveHandler);
    m_cameraHandler.updateBounds(600, 1080, ScreenBounds::AlignHorizontal::RIGHT, ScreenBounds::AlignVertical::TOP);
    m_touchLayer->addTouchHandler(&m_cameraHandler);
}

void InputComponent::setEntity(GameObject *gameObject) {
    m_gameObject = gameObject;
    active = true;
}

void InputComponent::update(float dt) {
    if (m_moveHandler.getActive()) {
        auto forceYDirection = m_camera->getDirection() - m_camera->getPosition();
        forceYDirection.normalize();
        auto forceXDirection = forceYDirection.cross(m_camera->getUpVector());
        forceXDirection.normalize();

        auto amplifier = 5.0f;
        physx::PxVec3 forceY = forceYDirection * m_moveHandler.getDirectionY() * amplifier;
        physx::PxVec3 forceX = forceXDirection * m_moveHandler.getDirectionX() * amplifier;
        auto force = forceX + forceY;
        force.y = 0.0f;
        m_gameObject->addForce(force);
    }
    if (m_cameraHandler.getActive()) {
        auto amplifier = 0.4f;
        auto deltaX = -m_cameraHandler.getMoveDeltaX() * amplifier;
        m_camera->rotateH(deltaX);
    }
}

void InputComponent::releaseEntity() {
    if (m_touchLayer != nullptr) {
        m_touchLayer->removeTouchHandler(&m_moveHandler);
    }
    m_gameObject = nullptr;
    active = false;
}
