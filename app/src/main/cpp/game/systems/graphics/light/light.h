#ifndef IRON_BALLS_LIGHT_H
#define IRON_BALLS_LIGHT_H

#include "../../../../glm/glm.hpp"

// TODO: delete
struct DirectionalLight
{
    glm::vec3 color;
    glm::vec3 direction;
    float diffuseIntensity;
};

struct DirectionalLightLocation {
    GLint color;
    GLint direction;
    GLint diffuseIntensity;
};

#endif //IRON_BALLS_LIGHT_H
