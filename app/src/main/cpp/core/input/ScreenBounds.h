#ifndef IRONBALLS_BOUNDS_H
#define IRONBALLS_BOUNDS_H

#include <math.h>

// TODO rename to ui bounds
// in pixels
struct ScreenBounds {
    enum AlignHorizontal {LEFT, CENTER, RIGHT};
    enum AlignVertical {TOP, MIDDLE, BOTTOM};

    AlignHorizontal alignHorizontal;
    AlignVertical alignVertical;
    float offsetX;
    float offsetY;
    int width;
    int height;

    bool contains(float x, float y, int screenWidth, int screenHeight);
};
#endif //IRONBALLS_BOUNDS_H
