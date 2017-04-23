#include "../../core/common.h"
#include <string>
#include "TouchInputHandler.h"

TouchInputHandler::TouchInputHandler() {
}

TouchInputHandler::TouchInputHandler(Type type, int zIndex) {
    m_type = type;
    m_zIndex = zIndex;
    m_pointerId = -1;
    m_bounds.alignHorizontal = ScreenBounds::AlignHorizontal::LEFT;
    m_bounds.alignVertical = ScreenBounds::AlignVertical::BOTTOM;
    m_bounds.width = 0;
    m_bounds.height = 0;
    m_startX = 0.0f;
    m_startY = 0.0f;
    m_directionX = 0.0f;
    m_directionY = 0.0f;
}

int TouchInputHandler::getPointerId() {
    return m_pointerId;
}

int TouchInputHandler::getType() {
    return m_type;
}

const int& TouchInputHandler::getZIndex() {
    return m_zIndex;
}

bool TouchInputHandler::getActive() {
    return m_pointerId >= 0;
}

float TouchInputHandler::getDirectionX() {
    return m_directionX;
}

float TouchInputHandler::getDirectionY() {
    return m_directionY;
}

void TouchInputHandler::updateBounds(int width, int height,
                                     ScreenBounds::AlignHorizontal hAlign,
                                     ScreenBounds::AlignVertical vAlign) {
    m_bounds.width = width;
    m_bounds.height = height;
    m_bounds.alignHorizontal = hAlign;
    m_bounds.alignVertical = vAlign;
}

bool TouchInputHandler::contains(float x, float y, int screenWidth, int screenHeight) {
    return m_bounds.contains(x, y, screenWidth, screenHeight);
}

void TouchInputHandler::start(int32_t pointerId, float x, float y) {
    m_pointerId = pointerId;
    m_startX = x;
    m_startY = y;

    std::string name = (m_type == Type::STICK) ? "Move handler " : " Attack handler ";
    name += " start";
    LOGD(name.c_str());
}

void TouchInputHandler::move(float x, float y) {
    m_directionX = x - m_startX;
    m_directionY = y - m_startY;
    float maxDirection = std::max(std::abs(m_directionX), std::abs(m_directionY));
    maxDirection = (maxDirection == 0.0f) ? 1.0f : maxDirection;
    m_directionX = m_directionX / maxDirection;
    m_directionY = m_directionY / maxDirection;
}

void TouchInputHandler::end() {
    m_pointerId = -1;

    std::string name = (m_type == Type::STICK) ? "Move handler " : " Attack handler ";
    name += " end";
    LOGD(name.c_str());
}

TouchInputHandler::~TouchInputHandler() {
}