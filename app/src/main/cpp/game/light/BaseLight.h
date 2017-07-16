#ifndef AWA_BASELIGHT_H
#define AWA_BASELIGHT_H

#include "../../glm/vec3.hpp"

struct BaseLight
{
    float ambientIntensity;
    float diffuseIntensity;
    glm::vec3 color;

    BaseLight()
    {
        ambientIntensity = 0.0f;
        diffuseIntensity = 0.0f;
        color = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

#endif //AWA_BASELIGHT_H
