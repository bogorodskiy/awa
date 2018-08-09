#include "PlayerController.h"
#include "../../../../libs/physx/foundation/PxVec3.h"
#include "GlobalSettings.h"

PlayerController::PlayerController() : m_moveHandler(TouchInputHandler::TouchInputType::STICK, 0),
                                       m_cameraHandler(TouchInputHandler::TouchInputType::STICK, 1) {

}

void PlayerController::initialize(TouchLayer *touchLayer, Camera *camera) {
    m_touchLayer = touchLayer;
    m_camera = camera;

    auto inputAreaWidth = static_cast<int>(GlobalSettings::DEFAULT_SCREEN_WIDTH * 0.5f);
    auto inputAreaHeight = GlobalSettings::DEFAULT_SCREEN_HEIGHT;

    m_moveHandler.updateBounds(inputAreaWidth, inputAreaHeight, ScreenBounds::AlignHorizontal::LEFT, ScreenBounds::AlignVertical::TOP);
    m_touchLayer->addTouchHandler(&m_moveHandler);
    m_cameraHandler.updateBounds(inputAreaWidth, inputAreaHeight, ScreenBounds::AlignHorizontal::RIGHT, ScreenBounds::AlignVertical::TOP);
    m_touchLayer->addTouchHandler(&m_cameraHandler);
}

void PlayerController::setPawn(std::shared_ptr<RigidDynamicComponent> pawn) {
    m_pawn = pawn;
}

void PlayerController::update() {
    if (m_pawn == nullptr) {
        return;
    }

    if (m_moveHandler.getActive()) {
        auto forceYDirection = m_camera->getDirection() - m_camera->getPosition();
        forceYDirection.normalize();
        auto forceXDirection = forceYDirection.cross(m_camera->getUpVector());
        forceXDirection.normalize();

        static const auto amplifier = 5.0f;
        physx::PxVec3 forceY = forceYDirection * m_moveHandler.getDirectionY() * amplifier;
        physx::PxVec3 forceX = forceXDirection * m_moveHandler.getDirectionX() * amplifier;
        auto force = forceX + forceY;
        force.y = 0.0f;
        m_pawn->addForce(force);
    }
    if (m_cameraHandler.getActive()) {
        auto amplifier = 0.4f;
        auto deltaX = -m_cameraHandler.getMoveDeltaX() * amplifier;
        auto deltaY = -m_cameraHandler.getMoveDeltaY() * amplifier;
        m_camera->rotate(deltaY, deltaX);
    }
}