#ifndef IRONBALLS_TOUCHINPUTHANDLER_H
#define IRONBALLS_TOUCHINPUTHANDLER_H

#include <stddef.h>
#include <stdint.h>
#include "ScreenBounds.h"

class TouchInputHandler {
public:
    enum Type {
        STICK,
        BUTTON
    };

    TouchInputHandler();
    TouchInputHandler(Type type, int zIndex);
    ~TouchInputHandler();

    int getPointerId();
    int getType();
    float getDirectionX();
    float getDirectionY();
    const int& getZIndex();
    bool getActive();
    void updateBounds(int width, int height, ScreenBounds::AlignHorizontal hAlign, ScreenBounds::AlignVertical vAlign);
    bool contains(float x, float y, int screenWidth, int screenHeight);

    void start(int32_t pointerId, float x, float y);
    void move(float x, float y);
    void end();

private:
    int32_t m_pointerId;
    Type m_type;
    ScreenBounds m_bounds;
    int m_zIndex;
    float m_startX;
    float m_startY;
    float m_directionX;
    float m_directionY;
};


#endif //IRONBALLS_TOUCHINPUTHANDLER_H
