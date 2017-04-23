#include <stdint.h>
#include "TouchLayer.h"

TouchLayer::TouchLayer() {
    m_screenWidth = 0;
    m_screenHeight = 0;
    m_minZIndex = 0;
    m_maxZIndex = 0;
}

int TouchLayer::getScreenWidth() {
    return m_screenWidth;
}

int TouchLayer::getScreenHeight() {
    return m_screenHeight;
}

void TouchLayer::updateScreenSize(const int &screenWidth, const int &screenHeight, float scaleFactor) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_scaleFactor = scaleFactor;
}

bool TouchLayer::processInputEvent(AInputEvent* event) {
    // TODO: move away android logic
    int32_t type = AInputEvent_getType(event);
    if (type == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event);
        int32_t flags = action & AMOTION_EVENT_ACTION_MASK;
        int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        int32_t pointerId = AMotionEvent_getPointerId(event, pointerIndex);
        float x = AMotionEvent_getX(event, pointerIndex) * m_scaleFactor;
        float y = m_screenHeight - AMotionEvent_getY(event, pointerIndex) * m_scaleFactor;

        TouchInputHandler* handler;
        switch (flags) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
            {
                tryStartHandlerForTouch(pointerId, x, y);
                break;
            }
            case AMOTION_EVENT_ACTION_MOVE:
            {
                handler = m_handlerByPointerId[pointerId];
                if (handler != nullptr) {
                    if (handler->getActive() && !handler->contains(x, y, m_screenWidth, m_screenHeight)) {
                        handler->end();
                    }
                    else if (!handler->getActive()&& handler->contains(x, y, m_screenWidth, m_screenHeight)) {
                        handler->start(pointerId, x, y);
                    }
                    else {
                        handler->move(x, y);
                    }
                }
                break;
            }
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
            case AMOTION_EVENT_ACTION_CANCEL:
            {
                handler = m_handlerByPointerId[pointerId];
                if (handler != nullptr) {
                    handler->end();
                    m_handlerByPointerId.erase(pointerId);
                }
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }
    else if (type == AINPUT_EVENT_TYPE_KEY) {
        LOGD("TouchLayer: Key input, key events are not implemented");
    }
    else {
        LOGD("TouchLayer: unknown input type");
    }
    return false;
}

bool TouchLayer::addTouchHandler(TouchInputHandler* handler) {
    std::vector<TouchInputHandler*>& handlers = m_touchHandlersByZIndex[handler->getZIndex()];
    for (auto& oldHandler : handlers) {
        if (handler == oldHandler) {
            LOGD("Unable to add touch handler, handler already added");
            return false;
        }
    }
    handlers.push_back(handler);
    m_minZIndex = std::min(m_minZIndex, handler->getZIndex());
    m_maxZIndex = std::max(m_maxZIndex, handler->getZIndex());
    return true;
}

bool TouchLayer::removeTouchHandler(TouchInputHandler* handler) {
    std::vector<TouchInputHandler*>& handlers = m_touchHandlersByZIndex[handler->getZIndex()];
    int nHandlers = handlers.size();
    for (int i = 0; i < nHandlers; i++) {
        auto& oldHandler = handlers.at(i);
        if (handler == oldHandler) {
            handlers.erase(handlers.begin() + i);
            return true;
        }
    }
    LOGD("Unable to delete touch handler, handler not found");
    return false;
}

void TouchLayer::tryStartHandlerForTouch(int32_t pointerId, float x, float y) {
    for (int index = m_maxZIndex; index >= m_minZIndex; index--) {
        std::vector<TouchInputHandler*>& handlers = m_touchHandlersByZIndex[index];
        for (auto& handler : handlers) {
            if (!handler->getActive() && handler->contains(x, y, m_screenWidth, m_screenHeight)) {
                handler->start(pointerId, x, y);
                m_handlerByPointerId[pointerId] = handler;
                return;
            }
        }
    }
}

TouchLayer::~TouchLayer() {
}