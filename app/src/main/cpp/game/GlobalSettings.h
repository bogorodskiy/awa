#ifndef AWA_CONFIG_H
#define AWA_CONFIG_H

#include <string>

class GlobalSettings {
public:
    static const int DEFAULT_SCREEN_WIDTH = 1920;
    static const int DEFAULT_SCREEN_HEIGHT = 1080;
    static const float NEAR_CLIPPING;
    static const float FAR_CLIPPING;
    static const float FIELD_OF_VIEW;
    static const std::string VERTEX_SHADER_PATH;
    static const std::string FRAGMENT_SHADER_PATH;
};

#endif //AWA_CONFIG_H
