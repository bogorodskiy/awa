#include "../../../core/common.h"
#include <string>
#include "TouchInputHandler.h"

TouchInputHandler::TouchInputHandler(TouchInputHandler::TouchInputType type, int zIndex):
        m_touched(false),
        m_released(true),
        m_type(type),
        m_zIndex(zIndex),
        m_pointerId(-1),
        m_startX(0.0f),
        m_startY(0.0f),
        m_directionX(0.0f),
        m_directionY(0.0f),
        m_deltaX(0.0f),
        m_deltaY(0.0f),
        m_prevX(0.0f),
        m_prevY(0.0f)
{
    m_bounds.alignHorizontal = ScreenBounds::AlignHorizontal::LEFT;
    m_bounds.alignVertical = ScreenBounds::AlignVertical::BOTTOM;
    m_bounds.width = 0;
    m_bounds.height = 0;
}

int TouchInputHandler::getPointerId() {
    return m_pointerId;
}

TouchInputHandler::TouchInputType TouchInputHandler::getType() {
    return m_type;
}

const int& TouchInputHandler::getZIndex() {
    return m_zIndex;
}

bool TouchInputHandler::getActive() {
    return m_pointerId >= 0;
}

bool TouchInputHandler::getAndResetTouched() {
    auto result = m_touched;
    m_touched = false;
    return result;
}

float TouchInputHandler::getDirectionX() {
    return m_directionX;
}

float TouchInputHandler::getDirectionY() {
    return m_directionY;
}

float TouchInputHandler::getMoveDeltaX() {
    return m_deltaX;
}

float TouchInputHandler::getMoveDeltaY() {
    return m_deltaY;
}

// TODO: check default width and real width
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

    m_prevX = x;
    m_prevY = y;

    m_touched = true;
    m_released = false;
}

void TouchInputHandler::move(float x, float y) {
    m_directionX = x - m_startX;
    m_directionY = y - m_startY;

    m_deltaX = x - m_prevX;
    m_deltaY = y - m_prevY;

    m_prevX = x;
    m_prevY = y;

    // normalize
    float maxDirection = std::max(std::abs(m_directionX), std::abs(m_directionY));
    maxDirection = (maxDirection == 0.0f) ? 1.0f : 1.0f / maxDirection;
    m_directionX = m_directionX * maxDirection;
    m_directionY = m_directionY * maxDirection;
}

void TouchInputHandler::end() {
    m_pointerId = -1;

    m_touched = false;
    m_released = true;
}

TouchInputHandler::~TouchInputHandler() {
}