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
        auto action = AMotionEvent_getAction(event);
        auto numPointers = AMotionEvent_getPointerCount(event);

        LOGD("processInputEvent");

        auto updateMove = false;
        TouchInputHandler* handler;
        auto flags = action & AMOTION_EVENT_ACTION_MASK;
        switch (flags) {
            // first touch
            case AMOTION_EVENT_ACTION_DOWN:
            {
                LOGD("AMOTION_EVENT_ACTION_DOWN");
                handleDownEvent(event, 0);
                break;
            }
            // second+ touch with spec index
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
            {
                LOGD("AMOTION_EVENT_ACTION_POINTER_DOWN");
                auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                handleDownEvent(event, pointerIndex);
                break;
            }
            case AMOTION_EVENT_ACTION_MOVE:
            {
                updateMove = true;
                //LOGD("MOVE EVENT POINTER %i", pointerId);
                break;
            }
            // all touches ended
            case AMOTION_EVENT_ACTION_UP: {
                LOGD("AMOTION_EVENT_ACTION_UP");
            }
            // touch ended with spec index
            case AMOTION_EVENT_ACTION_POINTER_UP: {
                LOGD("AMOTION_EVENT_ACTION_POINTER_UP");
                auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                handleUpEvent(event, pointerIndex);
                break;
            }
            case AMOTION_EVENT_ACTION_CANCEL:
            {
                LOGD("AMOTION_EVENT_ACTION_CANCEL");
            }
            default:
            {
                return false;
            }
        }

        if (updateMove) {
            for (auto id : m_activePointers) {
                auto index = findIndex(event, id);
                if (index != -1) {
                    float x = AMotionEvent_getX(event, index) * m_scaleFactor;
                    float y = m_screenHeight - AMotionEvent_getY(event, index) * m_scaleFactor;
                    handleMoveEvent(event, id, x, y);
                }
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

void TouchLayer::handleDownEvent(const AInputEvent* event, size_t pointerIndex) {
    auto pointerId = AMotionEvent_getPointerId(event, pointerIndex);
    m_activePointers.push_back(pointerId);
    LOGD("++ ACTIVE POINTER %i Added", pointerId);

    float x = AMotionEvent_getX(event, pointerIndex) * m_scaleFactor;
    float y = m_screenHeight - AMotionEvent_getY(event, pointerIndex) * m_scaleFactor;

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

void TouchLayer::handleMoveEvent(const AInputEvent* event, int32_t pointerId, float x, float y) {
    if (m_handlerByPointerId.find(pointerId) != m_handlerByPointerId.end()) {
        auto handler = m_handlerByPointerId[pointerId];

        if (handler->getActive()) {
            if (!handler->contains(x, y, m_screenWidth, m_screenHeight)) {
                handler->end();
            }
            else {
                handler->move(x, y);
            }
        }
        else if(handler->contains(x, y, m_screenWidth, m_screenHeight)) {
            handler->start(pointerId, x, y);
        }
    }
}

void TouchLayer::handleUpEvent(const AInputEvent* event, size_t pointerIndex) {
    auto pointerId = AMotionEvent_getPointerId(event, pointerIndex);
    if (m_handlerByPointerId.find(pointerId) != m_handlerByPointerId.end()) {
        auto handler = m_handlerByPointerId[pointerId];
        handler->end();
        m_handlerByPointerId.erase(pointerId);
    }
    for (auto it = m_activePointers.begin(); it != m_activePointers.end(); ++it) {
        if (*it == pointerId) {
            LOGD("++ ACTIVE POINTER %i Removed", pointerId);
            m_activePointers.erase(it);
            break;
        }
    }
}

int32_t TouchLayer::findIndex(const AInputEvent* event, int32_t pointerId) {
    auto count = AMotionEvent_getPointerCount(event);
    for (auto i = 0; i < count; ++i) {
        if (pointerId == AMotionEvent_getPointerId(event, i)) {
            return i;
        }
    }
    return -1;
}

TouchLayer::~TouchLayer() {
}