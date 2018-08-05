#ifndef IRONBALLS_INPUTSYSTEM_H
#define IRONBALLS_INPUTSYSTEM_H

#include <vector>
#include <map>
#include <android/input.h>
#include "../../../core/common.h"
#include "TouchInputHandler.h"

class TouchLayer {
public:
    TouchLayer();
    ~TouchLayer();

    int getScreenWidth();
    int getScreenHeight();
    void updateScreenSize(const int& screenWidth, const int& screenHeight, float scaleFactor);
    bool processInputEvent(AInputEvent* event);
    bool addTouchHandler(TouchInputHandler* handler);
    bool removeTouchHandler(TouchInputHandler* handler);
private:
    int m_screenWidth;
    int m_screenHeight;
    float m_scaleFactor;
    std::map<int, std::vector<TouchInputHandler*>> m_touchHandlersByZIndex;
    std::map<int32_t, TouchInputHandler*> m_handlerByPointerId;
    std::vector<int32_t> m_activePointers;
    int m_minZIndex;
    int m_maxZIndex;

    void handleDownEvent(const AInputEvent* event, size_t pointerIndex);
    void handleMoveEvent(const AInputEvent* event, int32_t pointerId, float x, float y);
    void handleUpEvent(const AInputEvent* event, size_t pointerIndex);

    int32_t findIndex(const AInputEvent* event, int32_t pointerId);
};


#endif //IRONBALLS_INPUTSYSTEM_H
