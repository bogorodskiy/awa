#ifndef AWA_POINTLIGHT_H
#define AWA_POINTLIGHT_H

#include "../../glm/vec3.hpp"
#include "BaseLight.h"

struct LightAttenuation {
    float constantFactor;
    float linearFactor;
    float exponentialFactor;
};

struct PointLight : public BaseLight {
    LightAttenuation attenuation;
    glm::vec3 position;

    PointLight() {
        // lightDistanceStrength = lightUnitDistanceStrength / (constantFactor + linearFactor * distance + exponentialFactor * distance * distance)

        attenuation.constantFactor = 1.0f;
        attenuation.linearFactor = 0.0f;
        attenuation.exponentialFactor = 0.0f;

        position = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

#endif //AWA_POINTLIGHT_H
