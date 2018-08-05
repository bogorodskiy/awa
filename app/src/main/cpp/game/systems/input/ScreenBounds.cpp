#include "ScreenBounds.h"

bool ScreenBounds::contains(float x, float y, int screenWidth, int screenHeight) {
    float minX = 0;
    float maxX = width;
    if (alignHorizontal == AlignHorizontal::CENTER) {
        minX = floor((screenWidth - width) * 0.5f);
        maxX = minX + width;
    }
    else if (alignHorizontal == AlignHorizontal::RIGHT) {
        minX = screenWidth - width;
        maxX = screenWidth;
    }

    float minY = 0;
    float maxY = height;
    if (alignVertical == AlignVertical ::MIDDLE) {
        minY = floor((screenHeight - height) * 0.5f);
        maxY = minY + height;
    }
    else if (alignVertical == AlignVertical::TOP) {
        minY = screenHeight - height;
        maxY = screenHeight;
    }

    return (x >= minX && x <= maxX) && (y >= minY && y <= maxY);
}