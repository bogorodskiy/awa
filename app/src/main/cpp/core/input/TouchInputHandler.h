#ifndef IRONBALLS_TOUCHINPUTHANDLER_H
#define IRONBALLS_TOUCHINPUTHANDLER_H

#include <stddef.h>
#include <stdint.h>
#include "ScreenBounds.h"

class TouchInputHandler {
public:
    enum class TouchInputType {
        STICK,
        BUTTON,
        POINTER
    };

    TouchInputHandler(TouchInputType type, int zIndex);
    ~TouchInputHandler();

    int getPointerId();
    TouchInputType getType();
    float getDirectionX();
    float getDirectionY();
    const int& getZIndex();
    bool getActive();
    bool getAndResetTouched();
    void updateBounds(int width, int height, ScreenBounds::AlignHorizontal hAlign, ScreenBounds::AlignVertical vAlign);
    bool contains(float x, float y, int screenWidth, int screenHeight);

    void start(int32_t pointerId, float x, float y);
    void move(float x, float y);
    void end();

private:
    TouchInputType m_type;
    int m_zIndex;
    int32_t m_pointerId;
    ScreenBounds m_bounds;
    float m_startX;
    float m_startY;
    float m_directionX;
    float m_directionY;
    bool m_touched;
    bool m_released;

    float m_prevX;
    float m_prevY;
};


#endif //IRONBALLS_TOUCHINPUTHANDLER_H
